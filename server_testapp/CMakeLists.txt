project(server_testapp)

find_package(OpenCV QUIET)
IF (OpenCV_FOUND)

    find_package(Boost 1.58.0 REQUIRED COMPONENTS thread log system)
    include_directories(${Boost_INCLUDE_DIRS})

    add_executable(server_testapp main.cpp)
    target_link_libraries(server_testapp server testRenderer)

    target_compile_definitions(server_testapp PUBLIC BOOST_LOG_DYN_LINK)

ENDIF ()