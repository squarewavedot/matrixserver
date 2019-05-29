cmake_minimum_required(VERSION 3.1)
project(Breakout3D)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS thread system)
include_directories(${Boost_INCLUDE_DIR})

set(MAINSRC
  src/breakoutgame.cpp
#  src/aplay.cpp
  src/main.cpp
)

set(MAINLIBS
#  libasound.so
  ${Boost_LIBRARIES}
)

add_executable(Breakout3D ${MAINSRC})
target_link_libraries(Breakout3D application ${MAINLIBS})