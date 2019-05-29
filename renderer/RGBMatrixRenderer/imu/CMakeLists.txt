project(imu)

set(SOURCE_FILES Imu.cpp SerialPort.cpp)

find_package(Boost 1.58.0 REQUIRED COMPONENTS thread log system)
include_directories(${Boost_INCLUDE_DIRS})

add_library(Imu SHARED ${SOURCE_FILES})
target_include_directories(Imu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Imu PRIVATE ${Boost_LIBRARIES})
#    target_include_directories(RGBMatrixRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})