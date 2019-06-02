#ifndef MATRIXSERVER_CUBEAPPLICATION_H
#define MATRIXSERVER_CUBEAPPLICATION_H


/*
TODO:
orthogonal (on screen) drawing functions:
- DrawCircle
- DrawRect
- FillCircle
- FillRect
other stuff:
- Render only changed parts of the Screen
- See if we can optimize the rendertime (currently around 20ms)
*/

#define CUBESIZE 64
#define CUBEMAXINDEX 63 //CUBESIZE - 1
#define CUBECENTER 32 //CUBESIZE / 2

#define VIRTUALCUBESIZE 66
#define VIRTUALCUBEMININDEX 1
#define VIRTUALCUBEMAXINDEX 65 //VIRTUALCUBESIZE - 1
#define VIRTUALCUBECENTER 33 //VIRTUALCUBESIZE / 2

#include "Color.h"
#include "MatrixApplication.h"
#include "Font6px.h"

#include <boost/multi_array.hpp>
#include <unistd.h>
#include <vector>
#include <string>
#include <Eigen/Dense>
#include <Eigen/StdVector>
using namespace Eigen;

typedef boost::multi_array<Color, 3> colorPixelArrayType;
typedef colorPixelArrayType::index colorPixelArrayTypeIndex;

/*
The Idea is to have a 3D volumetric representation of the whole cube. Like 64 by 64 by 64 "pixels".
To display the actual volumetric data, there shall be different modes:
- The simplest mode would be to just display the pixels on the surface of the volumetric representation
  soley by pushing them to the corresponding displays.
- Another Mode would implement some sort of "depth vision": pixels which are below the suface of one plane
  (up to a certain depth) would be transformed of some kind (like making them dimmer or acutally transforming them by perspective)
  and rendered to the corresponding displays.
- More modes to come
*/
enum ScreenNumber { front, right, back, left, top, bottom, anyScreen };
enum EdgeNumber { frontRight, rightBack, backLeft, leftFront, topFront, topRight, topBack, topLeft, bottomFront, bottomRight, bottomBack, bottomLeft, anyEdge };
enum CornerNumber { frontRightTop, rightBackTop, backLeftTop, leftFrontTop, frontRightBottom, rightBackBottom, backLeftBottom, leftFrontBottom, anyCorner };

class CubeApplication : public MatrixApplication{
public:
    CubeApplication(
            int fps = DEFAULTFPS,
            std::string setServerAddress = DEFAULTSERVERADRESS,
            std::string setServerPort = DEFAULTSERVERPORT);
    void setPixel3D(Vector3i pos, Color col, float intensity = 1.0f, bool add = false);
    void setPixel3D(int x, int y, int z, Color col, float intensity = 1.0f, bool add = false);
    void setPixelSmooth3D(Vector3f pos, Color color);
    void setPixelSmooth3D(float x, float y, float z, Color color);
    Color getPixel3D(Vector3i pos);
    Color getPixel3D(int x, int y, int z);
    bool isActivePixel3D(Vector3i pos);
    bool isActivePixel3D(int x, int y, int z);
    void fillAll(Color col);
    void render();
    void clear();
    void drawLine3D(int x1, int y1, int z1, const int x2, const int y2, const int z2, Color col);
    void drawLine3D(Vector3i start, Vector3i end, Color col);
    void drawText(ScreenNumber screenNr, Vector2i topLeftPoint, Color col, std::string text);
    void drawBitmap1bpp(ScreenNumber screenNr, Vector2i topLeftPoint, Color col, Bitmap1bpp bitmap);
//    void drawImage(ScreenNumber screenNr, Vector2i topLeftPoint, Magick::Image image);
    bool isOnSurface(Vector3i point);
    ScreenNumber getScreenNumber(Vector3i point);
    bool isOnEdge(Vector3i point);
    EdgeNumber getEdgeNumber(Vector3i point);
    bool isOnCorner(Vector3i point);
    CornerNumber getCornerNumber(Vector3i point);
    Vector3i getRandomPointOnScreen(ScreenNumber screenNr);
    Vector3i getPointOnScreen(ScreenNumber screenNr, Vector2i point);
    Vector3f getPointOnScreen(ScreenNumber screenNr, Vector2f point);
    virtual bool loop() = 0;
private:
    const int virtualSize_;
    const int virtualSizeAll_;
};

//constrain class
template<class T>
const T& constrain(const T& Value, const T& Min, const T& Max)
{
    return (Value < Min)? Min : (Value > Max)? Max : Value;
}


#endif //MATRIXSERVER_CUBEAPPLICATION_H
