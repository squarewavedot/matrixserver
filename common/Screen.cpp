#include <cstring>
#include "Screen.h"

Screen::Screen(int setWidth, int setHeight, int setScreenId) {
    screenId = setScreenId;
    width = setWidth;
    height = setHeight;
    screenDataSize = width*height;
    screenData.resize(width*height, 0x00);
    clear();
}

Screen::~Screen() {

}

int Screen::getScreenDataSize() {
    return screenDataSize;
}

std::vector<Color> & Screen::getScreenData() {
    return screenData;
}

Color * Screen::getScreenDataRaw() {
    return screenData.data();
}

void Screen::setScreenData(Color *data) {
    screenData.assign(data, data+screenDataSize);
}

void Screen::setScreenData(std::vector<Color> &newScreenData) {
    screenData = newScreenData;
}

int Screen::getWidth() {
    return width;
}

int Screen::getHeight() {
    return height;
}

int Screen::getArrayIndex(int x, int y) {
    return y + x * height;
}

void Screen::setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    setPixel(x, y, Color(red, green, blue));
}

void Screen::setPixel(int x, int y, Color col) {
    setPixel(x, y, col, false);
}

void Screen::setPixel(int x, int y, Color col, bool add) {
    if(x >= 0 && y >= 0 && x < width && y < height)
        screenData.at(getArrayIndex(x, y)) = add ? screenData.at(getArrayIndex(x, y)) + col : col;
}

Color Screen::getPixel(int x, int y){
    if(x >= 0 && y >= 0 && x < width && y < height)
        return screenData[getArrayIndex(x, y)];
    else
        return Color::black();
}

void Screen::clear() {
    std::memset(screenData.data(), 0, screenData.size() * sizeof(Color));
}

void Screen::fill(uint8_t red, uint8_t green, uint8_t blue) {
    std::fill(screenData.begin(), screenData.end(), Color(red, green, blue));
}

void Screen::fill(Color col) {
    fill(col.r(), col.g(), col.b());
}

int Screen::getScreenId() {
    return screenId;
}

void Screen::setScreenId(int newScreenId) {
    screenId = newScreenId;
}

void Screen::setRotation(Rotation newRotation) {
    rotation = newRotation;
}

Rotation Screen::getRotation() {
    return rotation;
}

void Screen::setOffsetX(int offset) {
    offsetX = offset;
}

int Screen::getOffsetX() {
    return offsetX;
}


void Screen::setOffsetY(int offset) {
    offsetY = offset;
}

int Screen::getOffsetY() {
    return offsetY;
}

void Screen::fade(float factor) {
    if(factor > 0.0f && factor < 1.0f){
        for(auto & color : screenData){
            color *= factor;
        }
    }
}

