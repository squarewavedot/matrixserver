project(MainMenu)
#
#find_package(Boost REQUIRED COMPONENTS thread system)
#include_directories(${Boost_INCLUDE_DIR})

set(MAINSRC
        main.cpp
        mainmenu.cpp)

set(MAINLIBS
#        ${Boost_LIBRARIES}
#        ${ImageMagick_LIBRARIES}
        stdc++fs
        )

add_executable(MainMenu ${MAINSRC})
target_link_libraries(MainMenu application ${MAINLIBS})


install(TARGETS MainMenu
        RUNTIME
        DESTINATION /home/pi/
        COMPONENT application
        )

