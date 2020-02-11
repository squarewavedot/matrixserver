project(application)

find_package(Boost 1.58.0 REQUIRED COMPONENTS thread log system)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

IF(APPLE)
	# Fix linking on 10.14+. See https://stackoverflow.com/questions/54068035
	# for sucessfull linking against lmlib2
	LINK_DIRECTORIES(/usr/local/lib)
ENDIF()

#set(CMAKE_CXX_FLAGS "-fPIC -I/opt/X11/include")

set(SOURCE_FILES
        CubeApplication.cpp
#        Mpu6050.cpp
#        Mpu6050.h ADS1000.cpp ADS1000.h
         Image.cpp)




add_library(matrixapplication SHARED ${SOURCE_FILES} MatrixApplication.cpp)
target_link_libraries(matrixapplication common renderer 
	$<$<PLATFORM_ID:Linux>:FPGARenderer>)
target_link_libraries(matrixapplication
        ${Boost_LIBRARIES} Imlib2
        $<$<PLATFORM_ID:Linux>:wiringPi> $<$<PLATFORM_ID:Linux>:rt> 
         )
target_include_directories(matrixapplication PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include/matrixapplication>)
target_include_directories(matrixapplication PUBLIC $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIRS}> $<INSTALL_INTERFACE:${EIGEN3_INCLUDE_DIRS}>)

target_compile_definitions(matrixapplication PUBLIC BOOST_LOG_DYN_LINK)
set_target_properties(matrixapplication PROPERTIES PUBLIC_HEADER "CubeApplication.h;Font6px.h;Mpu6050.h;ADS1000.h;Image.h;MatrixApplication.h")

#
#add_library(matrixapplicationstandalone SHARED ${SOURCE_FILES} MatrixApplicationStandalone.cpp)
#target_link_libraries(matrixapplicationstandalone common renderer FPGARenderer)
#target_link_libraries(matrixapplicationstandalone ${Boost_LIBRARIES} rt wiringPi Imlib2)
#target_include_directories(matrixapplicationstandalone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${EIGEN3_INCLUDE_DIRS})
#
#target_compile_definitions(matrixapplicationstandalone PUBLIC BOOST_LOG_DYN_LINK)
#target_compile_definitions(matrixapplicationstandalone PUBLIC MATRIXAPPLICATION_STANDALONE)
#

#set_target_properties(matrixapplicationstandalone PROPERTIES PUBLIC_HEADER "CubeApplication.h;Font6px.h;Mpu6050.h;ADS1000.h;Image.h;MatrixApplicationStandalone.h")

install(TARGETS matrixapplication
        EXPORT matrixapplication-targets
        LIBRARY
        DESTINATION lib
        COMPONENT matrixapplication
        PUBLIC_HEADER
        DESTINATION include/matrixapplication
        COMPONENT matrixapplication
        )

#install(TARGETS matrixapplicationstandalone
#        EXPORT matrixapplicationstandalone-targets
#        LIBRARY
#        DESTINATION lib
#        COMPONENT matrixapplicationstandalone
#        PUBLIC_HEADER
#        DESTINATION include
#        COMPONENT matrixapplicationstandalone
#        )
