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
        Joystick.cpp
        TcpServer.cpp TcpServer.h TcpClient.cpp TcpClient.h Cobs.cpp Cobs.h SocketConnection.cpp SocketConnection.h UnixSocketServer.cpp UnixSocketServer.h UnixSocketClient.cpp UnixSocketClient.h UniversalConnection.cpp UniversalConnection.h IpcServer.cpp IpcServer.h IpcConnection.cpp IpcConnection.h)

add_library(common STATIC ${SOURCE_FILES} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(common ${Protobuf_LIBRARIES})
target_link_libraries(common ${Boost_LIBRARIES} rt)
target_include_directories(common PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include/matrixapplication>)
target_include_directories(common PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> $<INSTALL_INTERFACE:lib>)
target_include_directories(common PUBLIC ${PROTOBUF_INCLUDE_DIRS})

target_compile_definitions(common PUBLIC PUBLIC BOOST_LOG_DYN_LINK)

set(HEADER_FILES
        Color.h
        Screen.h
        TcpServer.h
        TcpClient.h
        Cobs.h
        Joystick.h
        SocketConnection.h
        UnixSocketServer.h
        UnixSocketClient.h
        UniversalConnection.h
        IpcServer.h
        IpcConnection.h
        ${PROTO_HDRS}
        )

set_target_properties(common PROPERTIES PUBLIC_HEADER "Color.h;Screen.h;TcpServer.h;TcpClient.h;Cobs.h;SocketConnection.h;UnixSocketServer.h;UnixSocketClient.h;UniversalConnection.h;IpcServer.h;IpcConnection.h;Joystick.h;${PROTO_HDRS}")#;
##set_target_properties(commin PROPERTIES PUBLIC_HEADER "CubeApplication.h;Font6px.h;Joystick.h;Mpu6050.h;ADS1000.h;Image.h;MatrixApplication.h")
#install(FILES ${HEADER_FILES}
#        DESTINATION include)

install(TARGETS common
        EXPORT matrixapplication-targets
        ARCHIVE DESTINATION lib/static
        COMPONENT common
        PUBLIC_HEADER
        DESTINATION include/matrixapplication
        COMPONENT common
        )