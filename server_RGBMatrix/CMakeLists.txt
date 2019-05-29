project(server_RGBMatrix)

if (${BUILD_RASPBERRYPI})

    find_package(Boost 1.58.0 REQUIRED COMPONENTS thread log system)
    include_directories(${Boost_INCLUDE_DIRS})

    add_executable(server_RGBMatrix main.cpp)
    target_link_libraries(server_RGBMatrix server RGBMatrixRenderer simulatorRenderer rt)

    target_compile_definitions(server_RGBMatrix PUBLIC BOOST_LOG_DYN_LINK)

endif ()