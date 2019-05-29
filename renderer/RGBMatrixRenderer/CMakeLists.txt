project(RGBMatrixRenderer)

if (${BUILD_RASPBERRYPI})

    set(RGB_LIB_DISTRIBUTION rpi-rgb-led-matrix)
    set(RGB_INCDIR ${RGB_LIB_DISTRIBUTION}/include)
    set(RGB_LIBDIR ${RGB_LIB_DISTRIBUTION}/lib)
    set(RGB_LIBRARY_NAME rgbmatrix)

    include_directories(${RGB_INCDIR})
    link_directories(${RGB_LIBDIR})
    set(RGB_LIBRARIES ${RGB_LIBRARY_NAME} rt m)

    set(SOURCE_FILES
            RGBMatrixRenderer.cpp
            )

    add_library(RGBMatrixRenderer SHARED ${SOURCE_FILES})
    target_link_libraries(RGBMatrixRenderer PRIVATE renderer Imu ${RGB_LIBRARIES})
    target_include_directories(RGBMatrixRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_command(TARGET RGBMatrixRenderer PRE_LINK COMMAND "make" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/${RGB_LIB_DISTRIBUTION}/lib)
endif ()

add_subdirectory(imu)
