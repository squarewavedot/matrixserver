project(renderer)

set(SOURCE_FILES
        IRenderer.cpp
        )

add_library(renderer STATIC ${SOURCE_FILES})
target_link_libraries(renderer common)
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(TestRenderer)
add_subdirectory(SimulatorRenderer)
add_subdirectory(RGBMatrixRenderer)
add_subdirectory(FPGARenderer)