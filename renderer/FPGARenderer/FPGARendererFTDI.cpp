#include "FPGARendererFTDI.h"

#include <cstring>

extern "C" {
    #include "mpsse/mpsse.h"
}
//#include <bcm2835.h>
//#include <wiringPiSPI.h>

//#include "libMPSSE_spi.h"
//
//#define SPI_DEVICE_BUFFER_SIZE 256
//
//uint32 channels;
//FT_HANDLE ftHandle;
//ChannelConfig channelConf;
//uint8 buffer[SPI_DEVICE_BUFFER_SIZE];


// ---------------------------------------------------------
// icebreaker specific gpio functions
// ---------------------------------------------------------

static void set_cs(int cs_b)
{
    uint8_t gpio = 0;
    uint8_t direction = 0x2b;

    /*
     * XXX
     * The chip select here is the dedicated SPI chip select.
     * I am not sure how it is being toggled by hand yet.
     * Not sure this will work.
     */
    if (cs_b) {
        gpio |= 0x28;
    }

    mpsse_set_gpio(gpio, direction);
}

FPGARendererFTDI::FPGARendererFTDI() {

}

FPGARendererFTDI::FPGARendererFTDI(std::vector<std::shared_ptr<Screen>> initScreens) {
    init(initScreens);
}

void FPGARendererFTDI::init(std::vector<std::shared_ptr<Screen>> initScreens) {
    screens = initScreens;

    std::cout << "Init SPI Driver" << std::endl;
    mpsse_init(0, NULL, false);
}

void FPGARendererFTDI::setScreenData(int screenId, Color *screenData) {
    if(!renderMutex.try_lock())
        return;
    if (screenId < screens.size()) {
        screens.at(screenId)->setScreenData(screenData);
    }
    renderMutex.unlock();
}

void FPGARendererFTDI::render() {
    if(!renderMutex.try_lock())
        return;
    //Color tempPixelColor;

    const int screenWidth = screens[0]->getWidth();
    const int screenHeight = screens[0]->getHeight();
    const int bitDepth = sizeof(Color);
    const int screenCount = screens.size();

    int llen = screenWidth * bitDepth * screenCount;
    int flen = screenHeight * llen;
    uint8_t *buf = (uint8_t*)malloc(flen);

    uint8_t *cmd_buf = (uint8_t*)malloc(llen+128);
    /* Upload all the lines */
    for (int y=0; y<screenHeight; y++)
    {
        int i=0;

        /* Set CS low */
        cmd_buf[i++] = 0x80; /* MC_SETB_LOW */
        cmd_buf[i++] = 0x00; /* gpio */
        cmd_buf[i++] = 0x2b; /* dir  */

        /* SPI packet header */
        cmd_buf[i++] = 0x11; /* MC_DATA_OUT | MC_DATA_OCN */
        cmd_buf[i++] = (llen+1-1) & 0xff;
        cmd_buf[i++] = (llen+1-1) >> 8;

        /* SPI payload */
        cmd_buf[i++] = 0x80;

        Color tmpColor;
        for(const auto& screen : screens) {
            for(int x = 0; x < screen->getWidth(); x++) {
                switch (screen->getRotation()) {
                    case Rotation::rot0:
                            tmpColor = screen->getPixel(x, y);
                        break;
                    case Rotation::rot90:
                            tmpColor = screen->getPixel(screen->getHeight()-1-y, x);
                        break;
                    case Rotation::rot180:
                            tmpColor = screen->getPixel(screen->getWidth()-1-x, screen->getHeight()-1-y);
                        break;
                    case Rotation::rot270:
                            tmpColor = screen->getPixel(y, screen->getWidth()-1-x);
                        break;
                    default:
                        break;
                }
                cmd_buf[i + screen->getOffsetX() * (llen / screenCount) + x * bitDepth] = tmpColor.r();
                cmd_buf[i + screen->getOffsetX() * (llen / screenCount) + x * bitDepth + 1] = tmpColor.g();
                cmd_buf[i + screen->getOffsetX() * (llen / screenCount) + x * bitDepth + 2] = tmpColor.b();
            }
        }
        i += llen;

        /* Set CS high */
        cmd_buf[i++] = 0x80; /* MC_SETB_LOW */
        cmd_buf[i++] = 0x28; /* gpio */
        cmd_buf[i++] = 0x2b; /* dir  */

        /* Set CS low */
        cmd_buf[i++] = 0x80; /* MC_SETB_LOW */
        cmd_buf[i++] = 0x00; /* gpio */
        cmd_buf[i++] = 0x2b; /* dir  */

        /* SPI header */
        cmd_buf[i++] = 0x11; /* MC_DATA_OUT | MC_DATA_OCN */
        cmd_buf[i++] = 2-1;
        cmd_buf[i++] = 0;

        /* SPI payload */
        cmd_buf[i++] = 0x03;
        cmd_buf[i++] = y;

        /* Set CS high */
        cmd_buf[i++] = 0x80; /* MC_SETB_LOW */
        cmd_buf[i++] = 0x28; /* gpio */
        cmd_buf[i++] = 0x2b; /* dir  */

        mpsse_send_raw(cmd_buf, i);
    }

    /* Swap Frame */
    cmd_buf[0] = 0x04;
    cmd_buf[1] = 0x00;
    set_cs(0);
    mpsse_send_spi(cmd_buf, 2);
    set_cs(1);

    /* VSync */
#if 1
    do {
        cmd_buf[0] = 0x00;
        cmd_buf[1] = 0x00;
        set_cs(0);
        mpsse_xfer_spi(cmd_buf, 2);
        set_cs(1);
//        printf("%d\n", cmd_buf[0] | cmd_buf[1]);
    } while (((cmd_buf[0] | cmd_buf[1]) & 0x02) != 0x02);
#endif

    renderMutex.unlock();
}

void FPGARendererFTDI::setGlobalBrightness(int brightness) {
    if (brightness <= 100 && brightness >= 0) {
        globalBrightness = brightness;
    }
}

int FPGARendererFTDI::getGlobalBrightness() {
    return globalBrightness;
}
