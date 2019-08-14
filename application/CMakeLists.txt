project(application)

find_package(Boost 1.58.0 REQUIRED COMPONENTS thread log system)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-fPIC")

set(SOURCE_FILES
        CubeApplication.cpp
        Joystick.cpp
        Joystick.h
        Mpu6050.cpp
        Mpu6050.h ADS1000.cpp ADS1000.h
        Image.cpp)

add_library(application SHARED ${SOURCE_FILES} MatrixApplication.cpp)
target_link_libraries(application common renderer FPGARenderer)
target_link_libraries(application ${Boost_LIBRARIES} rt wiringPi Imlib2)
target_include_directories(application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${EIGEN3_INCLUDE_DIRS})

target_compile_definitions(application PUBLIC BOOST_LOG_DYN_LINK)

add_library(applicationstandalone SHARED ${SOURCE_FILES} MatrixApplicationStandalone.cpp)
target_link_libraries(applicationstandalone common renderer FPGARenderer)
target_link_libraries(applicationstandalone ${Boost_LIBRARIES} rt wiringPi Imlib2)
target_include_directories(applicationstandalone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${EIGEN3_INCLUDE_DIRS})

target_compile_definitions(applicationstandalone PUBLIC BOOST_LOG_DYN_LINK)
target_compile_definitions(applicationstandalone PUBLIC MATRIXAPPLICATION_STANDALONE)

set_target_properties(application PROPERTIES PUBLIC_HEADER "CubeApplication.h;Font6px.h;Joystick.h;Mpu6050.h;ADS1000.h;MatrixApplication.h")
set_target_properties(applicationstandalone PROPERTIES PUBLIC_HEADER "CubeApplication.h;Font6px.h;Joystick.h;Mpu6050.h;ADS1000.h;MatrixApplicationStandalone.h")

install(TARGETS application
        LIBRARY
        DESTINATION lib
        COMPONENT application
        PUBLIC_HEADER
        DESTINATION include
        COMPONENT application
        )

install(TARGETS applicationstandalone
        LIBRARY
        DESTINATION lib
        COMPONENT applicationstandalone
        PUBLIC_HEADER
        DESTINATION include
        COMPONENT applicationstandalone
        )