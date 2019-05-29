#include "TestRenderer.h"

TestRenderer::TestRenderer() {

}

TestRenderer::TestRenderer(std::vector<std::shared_ptr<Screen>> initScreens) {
    init(initScreens);
}

void TestRenderer::init(std::vector<std::shared_ptr<Screen>> initScreens) {
    screens = initScreens;
}

void TestRenderer::setScreenData(int screenId, Color *screenData) {
    if (screenId < screens.size()) {
        screens.at(screenId)->setScreenData(screenData);
    }
}

void TestRenderer::render() {
    for (auto screen : screens) {
        
    }
}

void TestRenderer::setGlobalBrightness(int brightness) {
    globalBrightness = brightness;
}

int TestRenderer::getGlobalBrightness() {
    return globalBrightness;
}
