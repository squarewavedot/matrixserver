#ifndef MATRIXSERVER_FPGARENDERERFTDI_H
#define MATRIXSERVER_FPGARENDERERFTDI_H

#include <IRenderer.h>
#include <mutex>
#include "Screen.h"

class FPGARendererFTDI : public IRenderer {
public:
    FPGARendererFTDI();

    FPGARendererFTDI(std::vector<std::shared_ptr<Screen>>);

    void init(std::vector<std::shared_ptr<Screen>>);

    void setScreenData(int, Color *);

    void render();

    void setGlobalBrightness(int);

    int getGlobalBrightness();

private:
    std::vector<std::shared_ptr<Screen>> screens;
    std::mutex renderMutex;
};

#endif //MATRIXSERVER_FPGARENDERERFTDI_H
