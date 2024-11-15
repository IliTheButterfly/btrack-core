

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

include_directories(${btrack_core_INCLUDES})

if (USE_CUDA)
	add_executable(
	  cuda_tests
	  cuda_tests.cpp
	)
	target_link_libraries(
	  cuda_tests
	  GTest::gtest_main
	  btrack-core-cuda
	)

	add_executable(
		processing_tests_with_cuda
		wrapper_tests.cpp
	)
	target_include_directories(processing_tests_with_cuda PUBLIC ${btrack_core_INCLUDES})
	target_link_libraries(
		processing_tests_with_cuda
		GTest::gtest_main
		btrack-core-cuda
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
)

include(GoogleTest)

if (USE_CUDA)
	gtest_discover_tests(cuda_tests)
	gtest_discover_tests(processing_tests_with_cuda)
endif ()

add_executable(
    processing_tests_no_cuda
    wrapper_tests.cpp
)
target_include_directories(processing_tests_no_cuda PUBLIC ${btrack_core_INCLUDES})
target_link_libraries(
    processing_tests_no_cuda
    GTest::gtest_main
    btrack-core
)

add_executable(
    settings_tests
    settings_tests.cpp
)
target_include_directories(settings_tests PUBLIC ${btrack_core_INCLUDES})
target_link_libraries(
    settings_tests
    GTest::gtest_main
    btrack-core
)

gtest_discover_tests(processing_tests_no_cuda)
gtest_discover_tests(opencv_tests)
gtest_discover_tests(settings_tests)


message(STATUS ${btrack_core_INCLUDES})
get_filename_component(test_file test.jpg ABSOLUTE)
message(STATUS ${test_file})
add_definitions(-DTEST_JPG_LOCATION="${test_file}")

