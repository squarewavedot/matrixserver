#include "CubeTest.h"

CubeTest::CubeTest() : CubeApplication(40, "cube3.local"){

}

bool CubeTest::loop() {
    drawText(ScreenNumber::front, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 0 front");
    drawText(ScreenNumber::right, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 1 right");
    drawText(ScreenNumber::back, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 2 back");
    drawText(ScreenNumber::left, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 3 left");
    drawText(ScreenNumber::top, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 4 top");
    drawText(ScreenNumber::bottom, Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), "Screen 5 bottom");
    render();
    return true;
}
