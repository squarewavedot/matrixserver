project(MainMenu)

find_package(matrixapplication REQUIRED)

set(MAINSRC
        main.cpp
        mainmenu.cpp)

set(MAINLIBS
        matrixapplication::matrixapplication
        stdc++fs
        )

add_executable(MainMenu ${MAINSRC})
target_link_libraries(MainMenu ${MAINLIBS})

install(TARGETS MainMenu DESTINATION bin)