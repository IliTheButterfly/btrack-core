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

project ("btrack")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
if (MSVC)
	set_property(GLOBAL PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

FetchContent_MakeAvailable(googletest)

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

# Include sub-projects.

file(GLOB Projects "btrack-*")

foreach(Project ${Projects})
	add_subdirectory(${Project})
endforeach(Project ${Projects})

include(Dart)
message(STATUS "USE CUDA: ${USE_CUDA}")

