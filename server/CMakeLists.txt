project(server)

find_package(Boost 1.58.0 REQUIRED COMPONENTS thread log system)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES
        Server.cpp
        App.cpp)

add_library(server STATIC ${SOURCE_FILES})
target_link_libraries(server common renderer application)
target_link_libraries(server ${Boost_LIBRARIES})
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(server PUBLIC BOOST_LOG_DYN_LINK)