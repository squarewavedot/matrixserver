#CMakeLists.txt
project(PixelFlow)

set(MAINSRC
    main.cpp pixelflow.cpp pixelflow.h)

set(MAINLIBS
  application
)

add_executable(PixelFlow ${MAINSRC})
target_link_libraries(PixelFlow ${MAINLIBS}) #-l flag
