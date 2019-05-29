project(SimulatorRenderer)

set(SOURCE_FILES
        SimulatorRenderer.cpp
        )

add_library(simulatorRenderer STATIC ${SOURCE_FILES})
target_link_libraries(simulatorRenderer renderer)
target_include_directories(simulatorRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})