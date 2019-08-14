#CMakeLists.txt
project(Picture)

set(MAINSRC
    main.cpp picture.cpp picture.h)

set(MAINLIBS
        application
)

add_executable(Picture ${MAINSRC})
target_link_libraries(Picture ${MAINLIBS} ) #-l flag