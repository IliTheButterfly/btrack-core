# CMakeList.txt : CMake project for btrack-core, include source and define
# project specific logic here.
#

# Add source to this project's executable.


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET btrack-core PROPERTY CXX_STANDARD 20)
#endif()
project ("btrack-core")

get_filename_component(local_dir ./btrack-core ABSOLUTE)
set(btrack_core_INCLUDES ${local_dir} GLOBAL_SCOPE)
include_directories(${btrack_core_INCLUDES})

# Find OpenCV and CUDA
find_package(OpenCV REQUIRED)
if (USE_CUDA)
find_package(CUDA REQUIRED)
endif ()
find_package(Boost COMPONENTS thread filesystem serialization REQUIRED)

# Include directories
# include_directories(${Boost_INCLUDE_DIR})
# include_directories(${OpenCV_INCLUDE_DIRS})

if (USE_CUDA)
	# add_definitions(-DUSE_CUDA)
	include_directories(${CUDA_INCLUDE_DIRS})
endif()


file(GLOB_RECURSE BTRACK_CORE_SOURCES "nodes/**.cpp" "processing/**.cpp" "settings/**.cpp" "btrack-core.cpp")
add_library(btrack-core ${BTRACK_CORE_SOURCES})

if (USE_CUDA)
	add_library (btrack-core-cuda ${SourceFiles})
	target_compile_definitions(btrack-core-cuda PUBLIC -DUSE_CUDA)
	if (MSVC)
		target_link_libraries(btrack-core-cuda ${CUDA_LIBRARIES} opencv_world ${Boost_LIBRARIES})
	else ()
		target_link_libraries(btrack-core-cuda ${CUDA_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})
	endif ()
endif()

if (MSVC)
	target_link_libraries(btrack-core opencv_world ${Boost_LIBRARIES})
else ()
	target_link_libraries(btrack-core ${OpenCV_LIBS} ${Boost_LIBRARIES})
endif ()


enable_testing()
add_subdirectory("tests")