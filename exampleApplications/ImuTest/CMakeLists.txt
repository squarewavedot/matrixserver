project(ImuTest)

find_package(Boost 1.58.0 REQUIRED COMPONENTS thread log system)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(imutestapp main.cpp ImuTest.cpp)
target_link_libraries(imutestapp application)

target_compile_definitions(imutestapp PUBLIC BOOST_LOG_DYN_LINK)