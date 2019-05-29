project(cubeTestApp)

find_package(Boost 1.58.0 REQUIRED COMPONENTS thread log system)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(cubetestapp main.cpp CubeTest.cpp)
target_link_libraries(cubetestapp application)

target_compile_definitions(cubetestapp PUBLIC BOOST_LOG_DYN_LINK)