cmake_minimum_required(VERSION 3.07)
project(matrixserver)


set(CMAKE_CXX_STANDARD 14)

#set(Boost_FIND_QUIETLY ON) #FINALLY!!!

if (EXISTS "/boot/LICENCE.broadcom") # I expect this file to be only found on an Raspian distribution
    set(BUILD_RASPBERRYPI true)
    message("Build for Raspberry Pi is enabled")
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

add_subdirectory(common)
add_subdirectory(renderer)
add_subdirectory(server)
add_subdirectory(application)
add_subdirectory(server_testapp)
add_subdirectory(server_RGBMatrix)
add_subdirectory(server_FPGA)
add_subdirectory(server_simulator)
add_subdirectory(exampleApplications)
add_subdirectory(tests)
