# CMakeList.txt : CMake project for btrack-core, include source and define
# project specific logic here.
#

# Add source to this project's executable.


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET btrack-core PROPERTY CXX_STANDARD 20)
#endif()


set(SourceFiles
	"btrack-core.cpp"
	"processing/camera.cpp"
	"processing/scene.cpp"
	"settings/SettingsManager.cpp"
	"nodes/MetaNode.cpp"
	"nodes/MetaNodeInputValue.cpp"
	"nodes/MetaNodeInputArray.cpp"
	"nodes/MetaNodeOutputValue.cpp"
	"nodes/MetaNodeOutputArray.cpp"

	"nodes/Node.cpp"
	"nodes/NodeInputValue.cpp"
	"nodes/NodeInputArray.cpp"
	"nodes/NodeOutputValue.cpp"
	"nodes/NodeOutputArray.cpp"
)


include_directories(${btrack_core_INCLUDES})
add_library (btrack-core ${SourceFiles})


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
