# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  if (MSVC)
  	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  endif ()
endif()

if (POLICY CMP0146)
	# cmake_policy(SET CMP0146 NEW)
endif()

project ("btrack-core")

get_filename_component(local_dir ./btrack-core ABSOLUTE)
set(btrack_core_INCLUDES ${local_dir} GLOBAL_SCOPE)
message(STATUS ${btrack_core_INCLUDES})
include_directories(${btrack_core_INCLUDES})

if (MSVC)
	add_compile_options(
		$<$<CONFIG:>:/MT>
		$<$<CONFIG:Debug>:/MT>
		$<$<CONFIG:RELEASE>:/MT>
	)
endif ()

# Define arguments
set(USE_CUDA OFF CACHE BOOL "Should CUDA be used?")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV and CUDA
find_package(OpenCV REQUIRED)
if (USE_CUDA)
find_package(CUDA REQUIRED)
endif ()
find_package(Boost COMPONENTS filesystem serialization REQUIRED)

# Include directories
message(STATUS ${Boost_INCLUDE_DIR})
message(STATUS ${Boost_LIBRARIES})
message(STATUS ${OpenCV_INCLUDE_DIRS})
message(STATUS ${OpenCV_LIBS})

# include_directories(${Boost_INCLUDE_DIR})
# include_directories(${OpenCV_INCLUDE_DIRS})

if (USE_CUDA)
	# add_definitions(-DUSE_CUDA)
	include_directories(${CUDA_INCLUDE_DIRS})
endif()


# Include sub-projects.
add_subdirectory ("btrack-core")

enable_testing()

add_subdirectory ("tests")

message(STATUS "USE CUDA: ${USE_CUDA}")

