﻿# CMakeList.txt : CMake project for btrack-core, include source and define
# project specific logic here.
#

# Add source to this project's executable.


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET btrack-core PROPERTY CXX_STANDARD 20)
#endif()
project ("btrack-core")


include_directories(${btrack_core_INCLUDES})
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