#ifndef MATRIXSERVER_IRENDERER_H
#define MATRIXSERVER_IRENDERER_H

#include <Color.h>
#include <Screen.h>
#include <vector>
#include <memory>

class IRenderer {
public:
    virtual void setScreenData(int, Color *) = 0;

    virtual void render() = 0;

    virtual void setGlobalBrightness(int) = 0;

    virtual int getGlobalBrightness() = 0;

protected:
    std::vector<std::shared_ptr<Screen>> screens;
    int globalBrightness = 100;
};


#endif //MATRIXSERVER_IRENDERER_H
