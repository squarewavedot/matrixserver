project(SimulatorRenderer)

set(SOURCE_FILES
        SimulatorRenderer.cpp
	#SimulatorRenderer.h
        )

#add_library(simulatorRenderer SHARED ${SOURCE_FILES})
add_library(simulatorRenderer STATIC ${SOURCE_FILES})
#target_link_libraries(simulatorRenderer PRIVATE renderer)
target_link_libraries(simulatorRenderer renderer)
target_include_directories(simulatorRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(simulatorRenderer PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include/matrixapplication>)

#set_target_properties(simulatorRenderer PROPERTIES PUBLIC_HEADER "SimulatorRenderer.h")
#
#install(TARGETS simulatorRenderer
#        EXPORT matrixapplication-targets
#        LIBRARY
#        DESTINATION lib
#        COMPONENT simulatorRenderer
#        PUBLIC_HEADER
#        DESTINATION include/matrixapplication
#        COMPONENT simulatorRenderer
#        )
