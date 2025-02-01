

include(GoogleTest)
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

add_executable(
    node_tests
    node_tests.cpp
	${SourceFiles}
)
target_include_directories(node_tests PUBLIC ${btrack_core_INCLUDES})
target_link_libraries(
    node_tests
    GTest::gtest_main
    btrack-core
)

add_executable(
    registry_tests
    registry_tests.cpp
)
target_include_directories(registry_tests PUBLIC ${btrack_core_INCLUDES})
target_link_libraries(
    registry_tests
    GTest::gtest_main
    btrack-core
)

# add_executable(
#     iterator_tests
#     iterator_tests.cpp
# )
# target_link_libraries(
#     iterator_tests
#     GTest::gtest_main
# )

set_tests_properties(${Tests} PROPERTIES TIMEOUT 10)

gtest_discover_tests(processing_tests_no_cuda)
gtest_discover_tests(opencv_tests)
gtest_discover_tests(settings_tests)
gtest_discover_tests(node_tests)
gtest_discover_tests(registry_tests)
# gtest_discover_tests(iterator_tests)


message(STATUS ${btrack_core_INCLUDES})
get_filename_component(test_file test.jpg ABSOLUTE)
message(STATUS ${test_file})
add_definitions(-DTEST_JPG_LOCATION="${test_file}")

