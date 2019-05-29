project(FPGARenderer)

set(SOURCE_FILES
        FPGARenderer.cpp
        )

include_directories(LibMPSSE-SPI_source/LibMPSSE-SPI/Release/include)
include_directories(LibMPSSE-SPI_source/LibMPSSE-SPI/Release/include/linux)
link_directories(LibMPSSE-SPI_source/LibMPSSE-SPI/LibMPSSE/Build/Linux)

add_library(FPGARenderer SHARED ${SOURCE_FILES})
target_link_libraries(FPGARenderer PRIVATE renderer MPSSE)
target_include_directories(FPGARenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(TARGET FPGARenderer PRE_LINK COMMAND "make" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/LibMPSSE-SPI_source/LibMPSSE-SPI/LibMPSSE/Build/Linux)