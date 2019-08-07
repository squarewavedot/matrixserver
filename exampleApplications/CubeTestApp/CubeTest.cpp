#include "CubeTest.h"

CubeTest::CubeTest() : CubeApplication(30){

}

bool CubeTest::loop() {
    static int loopcount = 0;
    clear();
//    if(loopcount%2 == 0){
//        fillAll(Color::white());
//    }
//    if(loopcount%2 == 0){
        drawLine3D(Vector3i(0,0,CUBESIZE-loopcount%CUBESIZE),Vector3i(CUBESIZE,0,CUBESIZE-loopcount%CUBESIZE), Color::red());
        drawLine3D(Vector3i(loopcount%CUBESIZE,0,CUBESIZE),Vector3i(loopcount%CUBESIZE,0,0), Color::blue());
//    }
    drawText(ScreenNumber::front, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 0 front");
    drawText(ScreenNumber::right, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 1 right");
    drawText(ScreenNumber::back, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 2 back");
    drawText(ScreenNumber::left, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 3 left");
    drawText(ScreenNumber::top, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 4 top");
    drawText(ScreenNumber::bottom, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 5 bottom");
    

    render();
    loopcount++;
    return true;
}