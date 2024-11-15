# CMakeList.txt : CMake project for btrack-core, include source and define
# project specific logic here.
#

# Add source to this project's executable.


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET btrack-core PROPERTY CXX_STANDARD 20)
#endif()



get_filename_component(local_dir ./ ABSOLUTE)
set(btrack_core_INCLUDES ${local_dir} PARENT_SCOPE)
include_directories(${btrack_core_INCLUDES})

add_library (btrack-core "btrack-core.cpp" "btrack-core.h" "processing/camera.h" "settings/CamSettings.h" "settings/paths.h" "settings/PTreeConvertible.h" "settings/SceneSettings.h" "settings/SettingsManager.h" "settings/SettingsManager.cpp")


if (USE_CUDA)
	add_library (btrack-core-cuda "btrack-core.cpp" "btrack-core.h" "processing/camera.h" "settings/CamSettings.h" "settings/paths.h" "settings/PTreeConvertible.h" "settings/SceneSettings.h" "settings/SettingsManager.h" "settings/SettingsManager.cpp")
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
# TODO: Add tests and install targets if needed.
