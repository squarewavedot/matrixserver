project(tests)

add_executable(testAll tests-cobs.cpp tests-main.cpp tests-screen.cpp tests-tcp.cpp test-unixSocket.cpp)
target_link_libraries(testAll common simulatorRenderer)