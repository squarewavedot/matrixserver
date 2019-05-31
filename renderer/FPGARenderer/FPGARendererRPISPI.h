#ifndef MATRIXSERVER_FPGARENDERERRPISPI_H
#define MATRIXSERVER_FPGARENDERERRPISPI_H


#include <IRenderer.h>
#include <mutex>
#include "Screen.h"

class FPGARendererRPISPI : public IRenderer {
public:
    FPGARendererRPISPI();

    FPGARendererRPISPI(std::vector<std::shared_ptr<Screen>>);

    void init(std::vector<std::shared_ptr<Screen>>);

    void setScreenData(int, Color *);

    void render();

    void setGlobalBrightness(int);

    int getGlobalBrightness();

private:
    std::vector<std::shared_ptr<Screen>> screens;
    std::mutex renderMutex;
    int spidevfilehandle;
};


#endif //MATRIXSERVER_FPGARENDERERRPISPI_H
