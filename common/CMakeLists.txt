project(common)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protobuf/matrixserver.proto)

find_package(Boost 1.58.0 REQUIRED COMPONENTS thread log system)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-fPIC")

set(SOURCE_FILES
        Color.cpp
        Screen.cpp
        TcpServer.cpp TcpServer.h TcpClient.cpp TcpClient.h Cobs.cpp Cobs.h SocketConnection.cpp SocketConnection.h UnixSocketServer.cpp UnixSocketServer.h UnixSocketClient.cpp UnixSocketClient.h UniversalConnection.cpp UniversalConnection.h IpcServer.cpp IpcServer.h IpcConnection.cpp IpcConnection.h)

add_library(common STATIC ${SOURCE_FILES} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(common ${Protobuf_LIBRARIES})
target_link_libraries(common ${Boost_LIBRARIES} rt)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(common PUBLIC ${PROTOBUF_INCLUDE_DIRS})

target_compile_definitions(common PUBLIC PUBLIC BOOST_LOG_DYN_LINK)