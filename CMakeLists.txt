cmake_minimum_required(VERSION 3.07)
project(matrixserver)


set(CMAKE_CXX_STANDARD 14)

set(Boost_FIND_QUIETLY ON) #FINALLY!!!

set(Boost_USE_MULTITHREADED TRUE) # fix warnings from newer boost versions

if (EXISTS "/boot/LICENCE.broadcom") # I expect this file to be only found on an Raspian distribution
    set(BUILD_RASPBERRYPI true)
    message("Build on Raspberry Pi is enabled")
endif ()

set(default_build_type "Release")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(common)
add_subdirectory(renderer)
add_subdirectory(server)
add_subdirectory(application)
if (BUILD_RASPBERRYPI)
add_subdirectory(MainMenu)
add_subdirectory(server_FPGA_RPISPI)
endif()
#add_subdirectory(server_testapp)
#add_subdirectory(server_RGBMatrix)
#add_subdirectory(server_FPGA)
add_subdirectory(server_simulator)
#add_subdirectory(tests)


install(EXPORT matrixapplication-targets
        NAMESPACE matrixapplication::
        FILE matrixapplication-config.cmake
        DESTINATION lib/cmake/matrixapplication)

install(PROGRAMS Update.sh DESTINATION sbin)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "LEDCube matrixserver dev files")
set(CPACK_PACKAGE_DESCRIPTION "LEDCube matrixserver & matrixapplication library")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")


set(CPACK_DEBIAN_PACKAGE_DEPENDS "libeigen3-dev,wiringpi,libboost-thread-dev,libboost-log-dev,libboost-system-dev,libboost-regex-dev,libboost-chrono-dev,libboost-date-time-dev,libboost-filesystem-dev,libboost-atomic-dev,libasound2-dev,libprotobuf-dev,protobuf-compiler,libimlib2-dev")
set(CPACK_PACKAGE_CONTACT "Adrian maintainer@squarewave.io")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local/")
#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/Debian/postinst")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
include(CPack)
