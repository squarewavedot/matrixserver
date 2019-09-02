project(server_FPGA_RPISPI)

find_package(matrixapplication REQUIRED)

add_executable(matrixserver main.cpp)
target_link_libraries(matrixserver server matrixapplication::FPGARenderer)

target_compile_definitions(matrixserver PUBLIC BOOST_LOG_DYN_LINK)

install(TARGETS matrixserver DESTINATION bin)