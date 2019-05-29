project(server_FPGA)

add_executable(server_FPGA main.cpp)
target_link_libraries(server_FPGA server FPGARenderer)

target_compile_definitions(server_FPGA PUBLIC BOOST_LOG_DYN_LINK)