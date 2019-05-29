#include "FPGARenderer.h"

#include "libMPSSE_spi.h"

#define SPI_DEVICE_BUFFER_SIZE 256

uint32 channels;
FT_HANDLE ftHandle;
ChannelConfig channelConf;
uint8 buffer[SPI_DEVICE_BUFFER_SIZE];

FPGARenderer::FPGARenderer() {

}

FPGARenderer::FPGARenderer(std::vector<std::shared_ptr<Screen>> initScreens) {
    init(initScreens);
}

void FPGARenderer::init(std::vector<std::shared_ptr<Screen>> initScreens) {
    screens = initScreens;

    FT_STATUS status;
    FT_DEVICE_LIST_INFO_NODE devList;
    uint8 address=0;
    uint16 data;
    int i,j;
    channelConf.ClockRate = 30000000;
    channelConf.LatencyTimer= 255;
    channelConf.configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS3;
    channelConf.Pin = 0x00000000;/*FinalVal-FinalDir-InitVal-InitDir (for dir 0=in, 1=out)*/
    status = SPI_GetNumChannels(&channels);
    printf("Number of available SPI channels = %d\n",channels);
    if(channels>0)
    {
        for(i=0;i<channels;i++)
        {
            status = SPI_GetChannelInfo(i,&devList);
            printf("Information on channel number %d:\n",i);
            printf("Flags=0x%x\n",devList.Flags);
            printf("Type=0x%x\n",devList.Type);
            printf("ID=0x%x\n",devList.ID);
            printf("LocId=0x%x\n",devList.LocId);
            printf("SerialNumber=%s\n",devList.SerialNumber);
            printf("Description=%s\n",devList.Description);
            printf("ftHandle=0x%x\n",devList.ftHandle);/*is 0 unless open*/
        }
        status = SPI_OpenChannel(0,&ftHandle);
        printf("\nhandle=0x%x status=0x%x\n",ftHandle,status);
        status = SPI_InitChannel(ftHandle,&channelConf);
        status = SPI_CloseChannel(ftHandle);
    }
}

void FPGARenderer::setScreenData(int screenId, Color *screenData) {
    if (screenId < screens.size()) {
        screens.at(screenId)->setScreenData(screenData);
    }
}

void FPGARenderer::render() {
    if(!renderMutex.try_lock())
        return;
    Color tempPixelColor;
//    auto usStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
    for (auto screen : screens) {
       /* auto ScreenData = screen->getScreenData();
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
        }*/
    }
//    auto usTotal = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()) - usStart;
//    std::cout << usTotal.count() << " us" << std::endl; // ~ 15ms

    renderMutex.unlock();
}

void FPGARenderer::setGlobalBrightness(int brightness) {
    if (brightness <= 100 && brightness >= 0) {
        globalBrightness = brightness;
    }
}

int FPGARenderer::getGlobalBrightness() {
    return globalBrightness;
}
