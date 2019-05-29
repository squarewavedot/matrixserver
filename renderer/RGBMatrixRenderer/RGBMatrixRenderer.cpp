#include "RGBMatrixRenderer.h"

#include "led-matrix.h"
#include "threaded-canvas-manipulator.h"
#include "transformer.h"

#include <chrono>

rgb_matrix::RGBMatrix::Options RGBmatrixOptions;
rgb_matrix::RuntimeOptions RGBruntimeOptions;
rgb_matrix::RGBMatrix *rgbMatrix;
rgb_matrix::FrameCanvas *rgbFrameCanvas;

RGBMatrixRenderer::RGBMatrixRenderer() {

}

RGBMatrixRenderer::RGBMatrixRenderer(std::vector<std::shared_ptr<Screen>> initScreens) {
    init(initScreens);
}

void RGBMatrixRenderer::init(std::vector<std::shared_ptr<Screen>> initScreens) {
    screens = initScreens;

    rgb_matrix::RGBMatrix::Options RGBmatrixOptions;
    rgb_matrix::RuntimeOptions RGBruntimeOptions;
    RGBmatrixOptions.hardware_mapping = "regular";
    RGBmatrixOptions.rows = 64;
    RGBmatrixOptions.cols = 64; //todo: should be tested -> tested now, no difference
    RGBmatrixOptions.chain_length = 3;
//    RGBmatrixOptions.chain_length = 6;
    RGBmatrixOptions.parallel = 2;
    RGBmatrixOptions.show_refresh_rate = false;
    RGBmatrixOptions.pwm_lsb_nanoseconds = 150;
    RGBmatrixOptions.brightness = globalBrightness;
    RGBmatrixOptions.led_rgb_sequence = "rgb";
    RGBruntimeOptions.gpio_slowdown = 2;
    RGBruntimeOptions.drop_privileges = -1;

    rgbMatrix = CreateMatrixFromOptions(RGBmatrixOptions, RGBruntimeOptions);

    rgbMatrix->set_luminance_correct(true);
    rgbFrameCanvas = rgbMatrix->CreateFrameCanvas();
    rgbFrameCanvas = rgbMatrix->SwapOnVSync(rgbFrameCanvas);
}

void RGBMatrixRenderer::setScreenData(int screenId, Color *screenData) {
    if (screenId < screens.size()) {
        screens.at(screenId)->setScreenData(screenData);
    }
}

void RGBMatrixRenderer::render() {
    if(!renderMutex.try_lock())
        return;
    Color tempPixelColor;
//    auto usStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
    for (auto screen : screens) {
        auto ScreenData = screen->getScreenData();
        switch (screen->getRotation()) {
            case Rotation::rot0:
                for (int i = 0, x = screen->getWidth() * screen->getOffsetX(); i < screen->getWidth(); i++, x++) {
                    for (int j = 0, y = screen->getHeight() * screen->getOffsetY(); j < screen->getHeight(); j++, y++) {
                        tempPixelColor = ScreenData[j + i * screen->getWidth()];
                        rgbFrameCanvas->SetPixel(x, y, tempPixelColor.r(), tempPixelColor.g(), tempPixelColor.b());
                    }
                }
                break;
            case Rotation::rot90:
                for (int i = 0, x = screen->getWidth() * screen->getOffsetX() + screen->getWidth() - 1; i < screen->getWidth(); i++, x--) {
                    for (int j = 0, y = screen->getHeight() * screen->getOffsetY(); j < screen->getHeight(); j++, y++) {
                        tempPixelColor = ScreenData[i + j * screen->getWidth()];
                        rgbFrameCanvas->SetPixel(x, y, tempPixelColor.r(), tempPixelColor.g(), tempPixelColor.b());
                    }
                }
                break;
            case Rotation::rot180:
                for (int i = 0, x = screen->getWidth() * screen->getOffsetX() + screen->getWidth() - 1;
                     i < screen->getWidth(); i++, x--) {
                    for (int j = 0, y = screen->getHeight() * screen->getOffsetY() + screen->getHeight() - 1;
                         j < screen->getHeight(); j++, y--) {
                        tempPixelColor = ScreenData[j + i * screen->getWidth()];
                        rgbFrameCanvas->SetPixel(x, y, tempPixelColor.r(), tempPixelColor.g(), tempPixelColor.b());
                    }
                }
                break;
            case Rotation::rot270:
                for (int i = 0, x = screen->getWidth() * screen->getOffsetX(); i < screen->getWidth(); i++, x++) {
                    for (int j = 0, y = screen->getHeight() * screen->getOffsetY() + screen->getHeight() - 1; j < screen->getHeight(); j++, y--) {
                        tempPixelColor = ScreenData[i + j * screen->getWidth()];
                        rgbFrameCanvas->SetPixel(x, y, tempPixelColor.r(), tempPixelColor.g(), tempPixelColor.b());
                    }
                }
                break;
            default:
                break;
        }
    }
//    auto usTotal = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()) - usStart;
//    std::cout << usTotal.count() << " us" << std::endl; // ~ 15ms
    rgbFrameCanvas = rgbMatrix->SwapOnVSync(rgbFrameCanvas);
    renderMutex.unlock();
}

void RGBMatrixRenderer::setGlobalBrightness(int brightness) {
    if (brightness <= 100 && brightness >= 0) {
        globalBrightness = brightness;
        rgbMatrix->SetBrightness(globalBrightness);
    }
}

int RGBMatrixRenderer::getGlobalBrightness() {
    return globalBrightness;
}
