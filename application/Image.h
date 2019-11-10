#ifndef IMAGE_H
#define IMAGE_H
#include <vector>
#include <Color.h>


namespace Imlib2{
#include <Imlib2.h>
}

class Image {
public:
    Image();
    bool loadImage(std::string filepath);
    Color at(int col, int row);

    int getWidth();
    int getHeight();
private:
    Imlib2::Imlib_Image image;
    std::vector<Color> imageData;
    unsigned int width;
    unsigned int height;
};


#endif //IMAGE_H
