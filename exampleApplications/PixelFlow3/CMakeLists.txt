#CMakeLists.txt
project(PixelFlow3)

set(MAINSRC
    main.cpp pixelflow.cpp pixelflow.h)

set(MAINLIBS
  application
)

add_executable(PixelFlow3 ${MAINSRC})
target_link_libraries(PixelFlow3 ${MAINLIBS}) #-l flag