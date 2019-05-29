#ifndef MATRIXSERVER_TESTRENDERER_H
#define MATRIXSERVER_TESTRENDERER_H

#include <IRenderer.h>
#include <opencv2/opencv.hpp>

class TestRenderer : public IRenderer {
public:
    TestRenderer();

    TestRenderer(std::vector<std::shared_ptr<Screen>>);

    void init(std::vector<std::shared_ptr<Screen>>);

    void setScreenData(int, Color *);

    void render();

    void setGlobalBrightness(int);

    int getGlobalBrightness();
};


#endif //MATRIXSERVER_TESTRENDERER_H
