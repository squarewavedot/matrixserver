project(server_simulator)

find_package(Boost 1.58.0 REQUIRED COMPONENTS thread log system)
include_directories(${Boost_INCLUDE_DIRS})

add_link_options(-undefined dynamic_lookup)

add_executable(server_simulator main.cpp)
target_link_libraries(server_simulator server simulatorRenderer $<$<PLATFORM_ID:Linux>:rt>)

target_compile_definitions(server_simulator PUBLIC BOOST_LOG_DYN_LINK)
