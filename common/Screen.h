#ifndef MATRIXSERVER_SCREEN_H
#define MATRIXSERVER_SCREEN_H

#include <vector>
#include "Color.h"

enum class Rotation {
    rot0, rot90, rot180, rot270
};


class Screen {
public:
    Screen(int setWidth, int setHeight, int setScreenId);

    ~Screen();

    int getWidth();

    int getHeight();

    Color getPixel(int x, int y);

    void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);

    void setPixel(int x, int y, Color col);

    void clear();

    void fill(uint8_t red, uint8_t green, uint8_t blue);

    void fill(Color col);

    std::vector<Color> & getScreenData();

    Color * getScreenDataRaw();

    void setScreenData(Color *data);

    void setScreenData(std::vector<Color> &);

    int getScreenDataSize();

    int getArrayIndex(int x, int y);

    int getScreenId();

    void setScreenId(int newScreenId);

    void setRotation(Rotation newRotation);

    Rotation getRotation();

    void setOffsetX(int offset);

    int getOffsetX();

    void setOffsetY(int offset);

    int getOffsetY();

protected:
    std::vector<Color> screenData;
    int screenDataSize;
    int screenId;
    int width;
    int height;
    int offsetX;
    int offsetY;
    Rotation rotation;
};


#endif //MATRIXSERVER_SCREEN_H