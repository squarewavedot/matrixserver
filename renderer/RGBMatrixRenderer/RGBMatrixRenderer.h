#ifndef MATRIXSERVER_RGBMATRIXRENDERER_H
#define MATRIXSERVER_RGBMATRIXRENDERER_H

#include <IRenderer.h>
#include <mutex>
#include "Screen.h"

class RGBMatrixRenderer : public IRenderer {
public:
    RGBMatrixRenderer();


    RGBMatrixRenderer(std::vector<std::shared_ptr<Screen>>);

    void init(std::vector<std::shared_ptr<Screen>>);

    void setScreenData(int, Color *);

    void render();

    void setGlobalBrightness(int);

    int getGlobalBrightness();

private:
    std::vector<std::shared_ptr<Screen>> screens;
    std::mutex renderMutex;
};


#endif //MATRIXSERVER_RGBMATRIXRENDERER_H
