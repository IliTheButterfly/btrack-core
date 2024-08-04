# CMakeList.txt : CMake project for btrack-core, include source and define
# project specific logic here.
#

# Add source to this project's executable.


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET btrack-core PROPERTY CXX_STANDARD 20)
#endif()


if (USE_CUDA)
	add_definitions(-DUSE_CUDA)
endif()

get_filename_component(local_dir ./ ABSOLUTE)
set(btrack_core_INCLUDES ${local_dir} PARENT_SCOPE)
include_directories(${btrack_core_INCLUDES})
add_library (btrack-core "btrack-core.cpp" "btrack-core.h" "processing/camera.h")


target_link_libraries(btrack-core ${CUDA_LIBRARIES} opencv_world)

# TODO: Add tests and install targets if needed.
