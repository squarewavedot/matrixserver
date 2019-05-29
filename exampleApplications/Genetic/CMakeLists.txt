project(genetic)

find_package(Boost 1.58.0 REQUIRED COMPONENTS thread log system)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(genetic main.cpp genetic.cpp)
target_link_libraries(genetic application)

target_compile_definitions(genetic PUBLIC BOOST_LOG_DYN_LINK)