#ifndef MATRIXSERVER_FPGARENDERER_H
#define MATRIXSERVER_FPGARENDERER_H

#include <IRenderer.h>
#include <mutex>
#include "Screen.h"

class FPGARenderer : public IRenderer {
public:
    FPGARenderer();

    FPGARenderer(std::vector<std::shared_ptr<Screen>>);

    void init(std::vector<std::shared_ptr<Screen>>);

    void setScreenData(int, Color *);

    void render();

    void setGlobalBrightness(int);

    int getGlobalBrightness();

private:
    std::vector<std::shared_ptr<Screen>> screens;
    std::mutex renderMutex;
};

#endif //MATRIXSERVER_FPGARENDERER_H
