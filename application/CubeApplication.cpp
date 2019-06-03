#include "CubeApplication.h"
#include <random>
#include <iostream>
#include <iomanip>


CubeApplication::CubeApplication(int fps, std::string setServerAddress, std::string setServerPort) :
        MatrixApplication(fps, setServerAddress, setServerPort),
        virtualSize_(VIRTUALCUBESIZE),
        virtualSizeAll_(VIRTUALCUBESIZE * VIRTUALCUBESIZE * VIRTUALCUBESIZE){
    clear();
    srand(time(NULL));
}

void CubeApplication::setPixel3D(Vector3i pos, Color col, float intensity, bool add) {
    return setPixel3D(pos[0], pos[1], pos[2], col, intensity, add);
}

void CubeApplication::setPixel3D(int x, int y, int z, Color col, float intensity, bool add) {
    x = constrain(x, 0, VIRTUALCUBEMAXINDEX);
    y = constrain(y, 0, VIRTUALCUBEMAXINDEX);
    z = constrain(z, 0, VIRTUALCUBEMAXINDEX);
    if (intensity != 1.0f)
        col *= intensity;

    if (y == 0)
        screens[0]->setPixel(x - 1, z - 1, col, add); //Screen 0: 3d_y = 0; x = 3d_x; y = 3d_z;
    else if (x == VIRTUALCUBEMAXINDEX)
        screens[1]->setPixel(y - 1, z - 1, col, add); //Screen 1: 3d_x = 63; 3d_y = x; 3d_z = y;
    else if (y == VIRTUALCUBEMAXINDEX)
        screens[2]->setPixel(VIRTUALCUBEMAXINDEX - x - 1, z - 1, col, add); //Screen 2: 3d_x = 63-x; 3d_y = 63; 3d_z = y;
    else if (x == 0)
        screens[3]->setPixel(VIRTUALCUBEMAXINDEX - y - 1, z - 1, col, add); //Screen 3: 3d_x = 0; 3d_y = 63-x; 3d_z = y;
    else if (z == 0)
        screens[4]->setPixel(x - 1, VIRTUALCUBEMAXINDEX - y - 1, col, add); //Screen 4: 3d_x = x; 3d_y = 63-y; 3d_z = 0;
    else if (z == VIRTUALCUBEMAXINDEX)
        screens[5]->setPixel(x - 1, y - 1, col, add); //Screen 5: 3d_x = x; 3d_y = y; 3d_z = 63;
}

void CubeApplication::setPixelSmooth3D(Vector3f pos, Color color) {
    return setPixelSmooth3D(pos[0], pos[1], pos[2], color);
}

void CubeApplication::setPixelSmooth3D(float x, float y, float z, Color color) {
    float rX = fmod(x, 1.0f);
    float rY = fmod(y, 1.0f);
    float rZ = fmod(z, 1.0f);
    int fX = (int) floor(x);
    int fY = (int) floor(y);
    int fZ = (int) floor(z);
    int cX = (int) ceil(x);
    int cY = (int) ceil(y);
    int cZ = (int) ceil(z);

    setPixel3D(fX, fY, fZ, color, (1.0f - rX) * (1.0f - rY) * (1.0f - rZ), true);
    if (rX != 0)setPixel3D(cX, fY, fZ, color, rX * (1.0f - rY) * (1.0f - rZ), true);
    if (rY != 0)setPixel3D(fX, cY, fZ, color, (1.0f - rX) * rY * (1.0f - rZ), true);
    if (rX != 0 && rY != 0)setPixel3D(cX, cY, fZ, color, rX * rY * (1.0f - rZ), true);
    if (rZ != 0)setPixel3D(fX, fY, cZ, color, (1.0f - rX) * (1.0f - rY) * rZ, true);
    if (rX != 0 && rZ != 0)setPixel3D(cX, fY, cZ, color, rX * (1.0f - rY) * rZ, true);
    if (rY != 0 && rZ != 0)setPixel3D(fX, cY, cZ, color, (1.0f - rX) * rY * rZ, true);
    if (rX != 0 && rY != 0 && rZ != 0)setPixel3D(cX, cY, cZ, color, rX * rY * rZ, true);
}

Color CubeApplication::getPixel3D(Vector3i pos) {
    return getPixel3D(pos[0], pos[1], pos[2]);
}

Color CubeApplication::getPixel3D(int x, int y, int z) {
    if (y == 0)
        return screens[0]->getPixel(x - 1, z - 1); //Screen 0: 3d_y = 0; x = 3d_x; y = 3d_z;
    else if (x == VIRTUALCUBEMAXINDEX)
        return screens[1]->getPixel(y - 1, z - 1); //Screen 1: 3d_x = 63; 3d_y = x; 3d_z = y;
    else if (y == VIRTUALCUBEMAXINDEX)
        return screens[2]->getPixel(VIRTUALCUBEMAXINDEX - x - 1, z - 1); //Screen 2: 3d_x = 63-x; 3d_y = 63; 3d_z = y;
    else if (x == 0)
        return screens[3]->getPixel(VIRTUALCUBEMAXINDEX - y - 1, z - 1); //Screen 3: 3d_x = 0; 3d_y = 63-x; 3d_z = y;
    else if (z == 0)
        return screens[4]->getPixel(x - 1, VIRTUALCUBEMAXINDEX - y - 1); //Screen 4: 3d_x = x; 3d_y = 63-y; 3d_z = 0;
    else if (z == VIRTUALCUBEMAXINDEX)
        return screens[5]->getPixel(x - 1, y - 1); //Screen 5: 3d_x = x; 3d_y = y; 3d_z = 63;
}

bool CubeApplication::isActivePixel3D(Vector3i pos) {
    return isActivePixel3D(pos[0], pos[1], pos[2]);
}

bool CubeApplication::isActivePixel3D(int x, int y, int z) {
    return getPixel3D(x, y, z) != Color::black();
}


void CubeApplication::fillAll(Color col) {
    for(auto & screen : screens){
        screen->fill(col);
    }
}

void CubeApplication::clear() {
    for(auto & screen : screens){
        screen->clear();
    }
}

void CubeApplication::render() {
    //nothing to do anymore
}

void CubeApplication::drawLine3D(Vector3i start, Vector3i end, Color col) {
    int l, m, n, err_1, err_2;
    Vector3i d1;
    Vector3i d2;
    Vector3i inc;
    Vector3i point;
    point = start;
    d1 = end - start;

    inc[0] = (d1[0] < 0) ? -1 : 1;
    inc[1] = (d1[1] < 0) ? -1 : 1;
    inc[2] = (d1[2] < 0) ? -1 : 1;
    l = abs(d1[0]);
    m = abs(d1[1]);
    n = abs(d1[2]);
    d2[0] = l << 1;
    d2[1] = m << 1;
    d2[2] = n << 1;

    if ((l >= m) && (l >= n)) {
        err_1 = d2[1] - l;
        err_2 = d2[2] - l;
        for (int i = 0; i < l; i++) {
            setPixel3D(point, col);
            if (err_1 > 0) {
                point[1] += inc[1];
                err_1 -= d2[0];
            }
            if (err_2 > 0) {
                point[2] += inc[2];
                err_2 -= d2[0];
            }
            err_1 += d2[1];
            err_2 += d2[2];
            point[0] += inc[0];
        }
    } else if ((m >= l) && (m >= n)) {
        err_1 = d2[0] - m;
        err_2 = d2[2] - m;
        for (int i = 0; i < m; i++) {
            setPixel3D(point, col);
            if (err_1 > 0) {
                point[0] += inc[0];
                err_1 -= d2[1];
            }
            if (err_2 > 0) {
                point[2] += inc[2];
                err_2 -= d2[1];
            }
            err_1 += d2[0];
            err_2 += d2[2];
            point[1] += inc[1];
        }
    } else {
        err_1 = d2[1] - n;
        err_2 = d2[0] - n;
        for (int i = 0; i < n; i++) {
            setPixel3D(point, col);
            if (err_1 > 0) {
                point[1] += inc[1];
                err_1 -= d2[2];
            }
            if (err_2 > 0) {
                point[0] += inc[0];
                err_2 -= d2[2];
            }
            err_1 += d2[1];
            err_2 += d2[0];
            point[2] += inc[2];
        }
    }
    setPixel3D(point, col);
}

void CubeApplication::drawLine3D(int x1, int y1, int z1, const int x2, const int y2, const int z2, Color col) {
    drawLine3D(Vector3i(x1, y1, z1), Vector3i(x2, y2, z2), col);
}

void CubeApplication::drawText(ScreenNumber screenNr, Vector2i topLeftPoint, Color col, std::string text) {
    Vector2i posIterator = topLeftPoint;
    if (topLeftPoint[0] == CharacterBitmaps::centered)
        posIterator[0] = (CUBESIZE - CharacterBitmaps::getStringWidth(text)) / 2;
    if (topLeftPoint[1] == CharacterBitmaps::centered)
        posIterator[1] = (CUBESIZE - CharacterBitmaps::fontHeight) / 2;
    if (topLeftPoint[0] == CharacterBitmaps::right)
        posIterator[0] = (CUBESIZE - CharacterBitmaps::getStringWidth(text));
    for (auto character : text) {
        Bitmap1bpp bitmap = CharacterBitmaps::getBitmapFromChar(character);
        drawBitmap1bpp(screenNr, posIterator, col, bitmap);
        posIterator += Vector2i(CharacterBitmaps::fontWidth, 0);
        if (posIterator[0] > CUBESIZE - CharacterBitmaps::fontWidth) {
            posIterator[1] += CharacterBitmaps::fontHeight;
            posIterator[0] = topLeftPoint[0];
        }
        if (posIterator[1] > CUBESIZE - CharacterBitmaps::fontHeight)
            break; //screen is full
    }
}

void CubeApplication::drawBitmap1bpp(ScreenNumber screenNr, Vector2i topLeftPoint, Color col, Bitmap1bpp bitmap) {
    for (auto pos : bitmap) {
        setPixel3D(getPointOnScreen(screenNr, Vector2i(topLeftPoint + pos)), col);
    }
}

//void CubeApplication::drawImage(ScreenNumber screenNr, Vector2i topLeftPoint, Magick::Image image){
//    for(uint cols = 0; cols < image.columns(); cols++){
//        for(uint rows = 0; rows < image.rows(); rows++){
//            Magick::Color col = image.pixelColor(cols, rows);
//            setPixel3D(getPointOnScreen(screenNr, Vector2i(cols, rows)), Color(col.redQuantum(), col.greenQuantum(), col.blueQuantum()));
//        }
//    }
//}

bool CubeApplication::isOnSurface(Vector3i point) {
    if (getScreenNumber(point) != anyScreen)
        return true;
    return false;
}

ScreenNumber CubeApplication::getScreenNumber(Vector3i point) {
    ScreenNumber result = anyScreen;
    if (point[0] == 0)
        result = left;
    else if (point[0] == VIRTUALCUBEMAXINDEX)
        result = right;
    else if (point[1] == 0)
        result = front;
    else if (point[1] == VIRTUALCUBEMAXINDEX)
        result = back;
    else if (point[2] == 0)
        result = top;
    else if (point[2] == VIRTUALCUBEMAXINDEX)
        result = bottom;
    return result;
}


bool CubeApplication::isOnEdge(Vector3i point) {
    if (getEdgeNumber(point) != anyEdge)
        return true;
    return false;
}

EdgeNumber CubeApplication::getEdgeNumber(Vector3i point) {
    EdgeNumber result = anyEdge;
    if (point[0] == VIRTUALCUBEMAXINDEX && point[1] == 0)
        result = frontRight;
    else if (point[0] == VIRTUALCUBEMAXINDEX && point[1] == VIRTUALCUBEMAXINDEX)
        result = rightBack;
    else if (point[0] == 0 && point[1] == VIRTUALCUBEMAXINDEX)
        result = backLeft;
    else if (point[0] == 0 && point[1] == 0)
        result = leftFront;
    else if (point[1] == 0 && point[2] == 0)
        result = topFront;
    else if (point[0] == VIRTUALCUBEMAXINDEX && point[2] == 0)
        result = topRight;
    else if (point[1] == VIRTUALCUBEMAXINDEX && point[2] == 0)
        result = topBack;
    else if (point[0] == 0 && point[2] == 0)
        result = topLeft;
    else if (point[1] == 0 && point[2] == VIRTUALCUBEMAXINDEX)
        result = bottomFront;
    else if (point[0] == VIRTUALCUBEMAXINDEX && point[2] == VIRTUALCUBEMAXINDEX)
        result = bottomRight;
    else if (point[1] == VIRTUALCUBEMAXINDEX && point[2] == VIRTUALCUBEMAXINDEX)
        result = bottomBack;
    else if (point[0] == 0 && point[2] == VIRTUALCUBEMAXINDEX)
        result = bottomLeft;
    return result;
}

//enum CornerNumber { frontRightTop, rightBackTop, backLeftTop, leftFrontTop, frontRightBottom, rightBackBottom, backLeftBottom, leftFrontBottom, anyCorner };
bool CubeApplication::isOnCorner(Vector3i point) {
    if (getCornerNumber(point) != anyCorner)
        return true;
    return false;
}

CornerNumber CubeApplication::getCornerNumber(Vector3i point) {
    CornerNumber result = anyCorner;
    if (point[0] == VIRTUALCUBEMAXINDEX && point[1] == 0 && point[2] == 0)
        result = frontRightTop;
    else if (point[0] == VIRTUALCUBEMAXINDEX && point[1] == VIRTUALCUBEMAXINDEX && point[2] == 0)
        result = rightBackTop;
    else if (point[0] == 0 && point[1] == VIRTUALCUBEMAXINDEX && point[2] == 0)
        result = backLeftTop;
    else if (point[0] == 0 && point[1] == 0 && point[2] == 0)
        result = leftFrontTop;
    else if (point[0] == VIRTUALCUBEMAXINDEX && point[1] == 0 && point[2] == VIRTUALCUBEMAXINDEX)
        result = frontRightBottom;
    else if (point[0] == VIRTUALCUBEMAXINDEX && point[1] == VIRTUALCUBEMAXINDEX && point[2] == VIRTUALCUBEMAXINDEX)
        result = rightBackBottom;
    else if (point[0] == 0 && point[1] == VIRTUALCUBEMAXINDEX && point[2] == VIRTUALCUBEMAXINDEX)
        result = backLeftBottom;
    else if (point[0] == 0 && point[1] == 0 && point[2] == VIRTUALCUBEMAXINDEX)
        result = leftFrontBottom;
    return result;
}

Vector3i CubeApplication::getRandomPointOnScreen(ScreenNumber screenNr) {
    Vector3i result;
    if (screenNr == anyScreen)
        screenNr = ScreenNumber(rand() % 6);
    switch (screenNr) {
        case anyScreen:
        case front:
            result = Vector3i(rand() % VIRTUALCUBEMAXINDEX, 0, rand() % VIRTUALCUBEMAXINDEX);
            break;
        case right:
            result = Vector3i(VIRTUALCUBEMAXINDEX, rand() % VIRTUALCUBEMAXINDEX, rand() % VIRTUALCUBEMAXINDEX);
            break;
        case back:
            result = Vector3i(rand() % VIRTUALCUBEMAXINDEX, VIRTUALCUBEMAXINDEX, rand() % VIRTUALCUBEMAXINDEX);
            break;
        case left:
            result = Vector3i(0, rand() % VIRTUALCUBEMAXINDEX, rand() % VIRTUALCUBEMAXINDEX);
            break;
        case top:
            result = Vector3i(rand() % VIRTUALCUBEMAXINDEX, rand() % VIRTUALCUBEMAXINDEX, 0);
            break;
        case bottom:
            result = Vector3i(rand() % VIRTUALCUBEMAXINDEX, rand() % VIRTUALCUBEMAXINDEX, VIRTUALCUBEMAXINDEX);
            break;
    }
    return result;
}

Vector3i CubeApplication::getPointOnScreen(ScreenNumber screenNr, Vector2i point) {
    Vector3i result;
    point += Vector2i(1, 1);
    switch (screenNr) {
        case front:
            result = Vector3i(point[0], 0, point[1]);
            break;
        case right:
            result = Vector3i(VIRTUALCUBEMAXINDEX, point[0], point[1]);
            break;
        case back:
            result = Vector3i(VIRTUALCUBEMAXINDEX - point[0], VIRTUALCUBEMAXINDEX, point[1]);
            break;
        case left:
            result = Vector3i(0, VIRTUALCUBEMAXINDEX - point[0], point[1]);
            break;
        case top:
            result = Vector3i(point[0], VIRTUALCUBEMAXINDEX - point[1], 0);
            break;
        case bottom:
            result = Vector3i(point[0], point[1], VIRTUALCUBEMAXINDEX);
            break;
        case anyScreen:
        default:
            break;
    }
    return result;
}


Vector3f CubeApplication::getPointOnScreen(ScreenNumber screenNr, Vector2f point) {
    Vector3f result;
    point += Vector2f(1, 1);
    switch (screenNr) {
        case front:
            result = Vector3f(point[0], 0, point[1]);
            break;
        case right:
            result = Vector3f(VIRTUALCUBEMAXINDEX, point[0], point[1]);
            break;
        case back:
            result = Vector3f(VIRTUALCUBEMAXINDEX - point[0], VIRTUALCUBEMAXINDEX, point[1]);
            break;
        case left:
            result = Vector3f(0, VIRTUALCUBEMAXINDEX - point[0], point[1]);
            break;
        case top:
            result = Vector3f(point[0], VIRTUALCUBEMAXINDEX - point[1], 0);
            break;
        case bottom:
            result = Vector3f(point[0], point[1], VIRTUALCUBEMAXINDEX);
            break;
        case anyScreen:
        default:
            break;
    }
    return result;
}

void CubeApplication::fade(float factor) {
    for(auto screen : screens)
        screen->fade(factor);
}
