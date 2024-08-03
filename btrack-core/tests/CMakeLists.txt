

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# set_property(GLOBAL PROPERTY
# 	MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

FetchContent_MakeAvailable(googletest)



enable_testing()



if (USE_CUDA)
	add_executable(
	  cuda_tests
	  cuda_tests.cpp
	)
	target_link_libraries(
	  cuda_tests
	  GTest::gtest_main
	  btrack-core
	  opencv_world
	)
endif ()


add_executable(
	opencv_tests
	opencv_tests.cpp
)
target_link_libraries(
	opencv_tests
	GTest::gtest_main
	btrack-core
	opencv_world
)

include(GoogleTest)

if (USE_CUDA)
	gtest_discover_tests(cuda_tests)
endif ()

GTEST_DISCOVER_TESTS(opencv_tests)

set(test_dir ${CMAKE_BUILD_TYPE})
configure_file(test.jpg ${test_dir}/test.jpg COPYONLY)
