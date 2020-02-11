project(TestRenderer)

find_package(OpenCV QUIET)
IF (OpenCV_FOUND)
    link_directories(${OpenCV_LIB_DIR})

    set(SOURCE_FILES
            TestRenderer.cpp
            TestRenderer.h
            )

    add_library(testRenderer STATIC ${SOURCE_FILES})
    target_link_libraries(testRenderer renderer ${OpenCV_LIBS})
    target_include_directories(testRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF ()