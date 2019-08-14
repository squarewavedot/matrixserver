#include "Image.h"
#include <iostream>

Image::Image() {

}

void Image::loadImage(std::string filepath) {
    image = Imlib2::imlib_load_image(filepath.data());
    Imlib2::imlib_context_set_image(image);
    width = Imlib2::imlib_image_get_width();
    height = Imlib2::imlib_image_get_height();
    imageData.resize(width * height, Color::black());

    for (uint cols = 0; cols < width; cols++) {
        for (uint rows = 0; rows < height; rows++) {
            Imlib2::Imlib_Color tempColor;
            Imlib2::imlib_image_query_pixel(cols, rows, &tempColor);
            imageData[rows + cols * height] = Color(tempColor.red,tempColor.green,tempColor.blue);
//            std::cout << cols << ", " << rows << ": " << tempColor.red << " " << tempColor.green << " " << tempColor.blue << std::endl;
        }
    }
}

Color Image::at(int col, int row) {
    int idx = row + col * height;
//    return imageData[idx];
    if (idx < imageData.size())
        return imageData[idx];
    else
        return imageData[0];
}

int Image::getHeight() {
    return height;
}

int Image::getWidth() {
    return width;
}
