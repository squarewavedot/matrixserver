#include "FPGARendererRPISPI.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <errno.h>
#include <cstring>
#include <thread>

const char *spiDevice = "/dev/spidev0.0";
uint8_t spiMode = 0;
uint8_t spiBits = 8;
uint32_t spiSpeed = 65000000;
uint16_t spiDelay = 1;
int spiDevFilehandle;

int screenWidth, screenHeight, bitDepthInBytes, screenCount, bytesPerLine;
unsigned char *cmd_buf;

struct spi_ioc_transfer * spiIocTransfers;
unsigned int spiIocTransfersSize, spiIocTransfersPos;
unsigned char *spiWriteQueueBuffer;
unsigned char *spiWriteQueueBufferCurrentPos;

unsigned char spiWriteQueueBackBuffer0[65535];  //49346 needed
unsigned char spiWriteQueueBackBuffer1[65535];
struct spi_ioc_transfer transferBackBuffer0[255];
struct spi_ioc_transfer transferBackBuffer1[255];
int buffPos = 0;

bool SpiWriteQueueInit(unsigned int count, unsigned int bufferSize){
    if(buffPos++%2){
        spiWriteQueueBuffer = spiWriteQueueBackBuffer0;
        spiIocTransfers = transferBackBuffer0;
    }
    else{
        spiWriteQueueBuffer = spiWriteQueueBackBuffer1;
        spiIocTransfers = transferBackBuffer1;
    }
//    spiWriteQueueBuffer = (unsigned char*)malloc(bufferSize);//49346
    spiWriteQueueBufferCurrentPos = spiWriteQueueBuffer;

//    spiIocTransfers = (spi_ioc_transfer *)malloc(sizeof(struct spi_ioc_transfer) * count);
    memset (spiIocTransfers, 0, sizeof(struct spi_ioc_transfer) * count);
    spiIocTransfersSize = count;
    spiIocTransfersPos = 0;
};

bool SpiWriteQueueAdd(unsigned char * data, unsigned int length) {
    if(spiIocTransfersPos < spiIocTransfersSize){
        memcpy(spiWriteQueueBufferCurrentPos, data, length);
        memset (&spiIocTransfers[spiIocTransfersPos], 0, sizeof (spi_ioc_transfer));
        spiIocTransfers[spiIocTransfersPos].tx_buf        = (unsigned long)spiWriteQueueBufferCurrentPos;
        spiIocTransfers[spiIocTransfersPos].rx_buf        = (unsigned long)spiWriteQueueBufferCurrentPos;
        spiIocTransfers[spiIocTransfersPos].len           = length;
        spiIocTransfers[spiIocTransfersPos].delay_usecs   = spiDelay;
        spiIocTransfers[spiIocTransfersPos].speed_hz      = spiSpeed;
        spiIocTransfers[spiIocTransfersPos].bits_per_word = spiBits;
        spiIocTransfers[spiIocTransfersPos].cs_change     = true;
        spiWriteQueueBufferCurrentPos += length;
        spiIocTransfersPos++;
        return true;
    }else{
        std::cout << "SpiWriteQueue not large enough, omitting data!" << std::endl;
        return false;
    }
}

bool SpiWriteQueueAddCSTrigger(){
    if(spiIocTransfersPos < spiIocTransfersSize){
        spiIocTransfers[spiIocTransfersPos].tx_buf        = NULL;
        spiIocTransfers[spiIocTransfersPos].rx_buf        = NULL;
        spiIocTransfers[spiIocTransfersPos].len           = 0;
        spiIocTransfers[spiIocTransfersPos].delay_usecs   = spiDelay;
        spiIocTransfers[spiIocTransfersPos].speed_hz      = spiSpeed;
        spiIocTransfers[spiIocTransfersPos].bits_per_word = spiBits;
        spiIocTransfers[spiIocTransfersPos].cs_change     = true;
        spiIocTransfersPos++;
        return true;
    }else{
        std::cout << "SpiWriteQueue not large enough, omitting data!" << std::endl;
        return false;
    }
}

void SpiWriteQueueTrigger(){
    std::thread([&](){
        ioctl (spiDevFilehandle, SPI_IOC_MESSAGE(spiIocTransfersPos), spiIocTransfers);
    }).detach();
}

int SpiWriteRead(unsigned char *data, unsigned int length) {
    struct spi_ioc_transfer spi;
    memset (&spi, 0, sizeof (spi));

    spi.tx_buf        = (unsigned long)data;
    spi.rx_buf        = (unsigned long)data;
    spi.len           = length;
    spi.delay_usecs   = spiDelay;
    spi.speed_hz      = spiSpeed;
    spi.bits_per_word = spiBits;
    spi.cs_change     = false;

    return ioctl (spiDevFilehandle, SPI_IOC_MESSAGE(1), &spi) ;
}


FPGARendererRPISPI::FPGARendererRPISPI() {

}

FPGARendererRPISPI::FPGARendererRPISPI(std::vector<std::shared_ptr<Screen>> initScreens) {
    init(initScreens);
}

void FPGARendererRPISPI::init(std::vector<std::shared_ptr<Screen>> initScreens) {
    screens = initScreens;

    screenWidth = screens[0]->getWidth();
    screenHeight = screens[0]->getHeight();
    bitDepthInBytes = 2;
    screenCount = screens.size();
    bytesPerLine = screenWidth * bitDepthInBytes * screenCount;
    cmd_buf = (unsigned char*)malloc(bytesPerLine+128);
//    spiWriteQueueBuffer = (unsigned char*)malloc(49346); //hacky test

    initSpi();
}

bool FPGARendererRPISPI::initSpi() const {
    int ret;
    std::cout << "Init SPI Driver" << std::endl;

    /* Device oeffen */
    if ((spiDevFilehandle = open(spiDevice, O_RDWR)) < 0) {
        perror("Fehler Open Device");
        exit(1);
    }
/* Mode setzen */
    ret = ioctl(spiDevFilehandle, SPI_IOC_WR_MODE, &spiMode);
    if (ret < 0) {
        perror("Fehler Set SPI-Modus");
        exit(1);
    }

/* Mode abfragen */
    ret = ioctl(spiDevFilehandle, SPI_IOC_RD_MODE, &spiMode);
    if (ret < 0) {
        perror("Fehler Get SPI-Modus");
        exit(1);
    }

/* Wortlaenge setzen */
    ret = ioctl(spiDevFilehandle, SPI_IOC_WR_BITS_PER_WORD, &spiBits);
    if (ret < 0) {
        perror("Fehler Set Wortlaenge");
        exit(1);
    }

/* Wortlaenge abfragen */
    ret = ioctl(spiDevFilehandle, SPI_IOC_RD_BITS_PER_WORD, &spiBits);
    if (ret < 0) {
        perror("Fehler Get Wortlaenge");
        exit(1);
    }

/* Datenrate setzen */
    ret = ioctl(spiDevFilehandle, SPI_IOC_WR_MAX_SPEED_HZ, &spiSpeed);
    if (ret < 0) {
        perror("Fehler Set Speed");
        exit(1);
    }

/* Datenrate abfragen */
    ret = ioctl(spiDevFilehandle, SPI_IOC_RD_MAX_SPEED_HZ, &spiSpeed);
    if (ret < 0) {
        perror("Fehler Get Speed");
        exit(1);
    }

/* Kontrollausgabe */
    printf("SPI-Device.....: %s\n", spiDevice);
    printf("SPI-Mode.......: %d\n", spiMode);
    printf("Wortlaenge.....: %d\n", spiBits);
    printf("Geschwindigkeit: %d Hz (%d MHz)\n", spiSpeed, spiSpeed / 1000000);
}

void FPGARendererRPISPI::setScreenData(int screenId, Color *screenData) {
    if(!screenDataMutex.try_lock())
        return;
    if (screenId < screens.size()) {
        screens.at(screenId)->setScreenData(screenData);
    }
    screenDataMutex.unlock();
}

void FPGARendererRPISPI::render() {
    if(!screenDataMutex.try_lock())
        return;

    auto usStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());

    // 2 transfers per line (linedata + line flush) + 1 frame swap, 3 additional bytes (startbyte + line flush) per linelength + 2 bytes for frameswap
    SpiWriteQueueInit(screenHeight * 2 + 1, (bytesPerLine+3) * screenHeight + 2);

    /* Upload all the lines */
    for (int y=0; y<screenHeight; y++)
    {
        int i=0;
        /* SPI payload */
        cmd_buf[i++] = 0x80;

        Color tmpColor;
        uint16_t *pixelP;
        for(const auto& screen : screens) {
            Color * screenData = screen->getScreenDataRaw();
            for(int x = 0; x < screen->getWidth(); x++) {
                switch (screen->getRotation()) {
                    case Rotation::rot0:
//                        tmpColor = screen->getPixel(x, y);
                        tmpColor = screenData[y + x * screenHeight];
                        break;
                    case Rotation::rot90:
//                        tmpColor = screen->getPixel(screenHeight-1-y, x);
                        tmpColor = screenData[x + (screenHeight-1-y) * screenHeight];
                        break;
                    case Rotation::rot180:
//                        tmpColor = screen->getPixel(screenWidth-1-x, screenHeight-1-y);
                        tmpColor = screenData[screenWidth-1-y + (screenHeight-1-x) * screenHeight];
                        break;
                    case Rotation::rot270:
//                        tmpColor = screen->getPixel(y, screenWidth-1-x);
                        tmpColor = screenData[screenWidth-1-x + y * screenHeight];
                        break;
                    default:
                        break;
                }
                //bitDepthInBytes == 2
                pixelP = (uint16_t *)(cmd_buf + i + screen->getOffsetX() * (bytesPerLine / screenCount) + x * bitDepthInBytes);
                *pixelP = tmpColor.r() >> 3 << 11 | tmpColor.g() >> 2 << 6 | tmpColor.b() >> 3;
                //bitDepthInBytes == 3
//                cmd_buf[i + screen->getOffsetX() * (bytesPerLine / screenCount) + x * bitDepthInBytes] = tmpColor.r();
//                cmd_buf[i + screen->getOffsetX() * (bytesPerLine / screenCount) + x * bitDepthInBytes + 1] = tmpColor.g();
//                cmd_buf[i + screen->getOffsetX() * (bytesPerLine / screenCount) + x * bitDepthInBytes + 2] = tmpColor.b();
            }
        }
        i += bytesPerLine;
        SpiWriteQueueAdd(cmd_buf, i);

        //Line flush command
        cmd_buf[0] = 0x03;
        cmd_buf[1] = y;
        SpiWriteQueueAdd(cmd_buf, 2);
//        SpiWriteQueueAddCSTrigger();
    }


    /* Swap Frame */
    cmd_buf[0] = 0x04;
    cmd_buf[1] = 0x00;
    SpiWriteQueueAdd(cmd_buf, 2);

    /* Doing VSync first */
    do {
        cmd_buf[0] = 0x00;
        cmd_buf[1] = 0x00;
        SpiWriteRead(cmd_buf, 2);
//        printf("%d\n", cmd_buf[0] | cmd_buf[1]);
    } while (((cmd_buf[0] | cmd_buf[1]) & 0x02) != 0x02);

//    auto usTotal2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()) - usStart;
//    std::cout << "vsync:  " << usTotal2.count() << " us" << std::endl;

    SpiWriteQueueTrigger();

    auto usTotal = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()) - usStart;
    std::cout << "render: " << usTotal.count() << " us" << std::endl;
    screenDataMutex.unlock();
}

void FPGARendererRPISPI::setGlobalBrightness(int brightness) {
    if (brightness <= 100 && brightness >= 0) {
        globalBrightness = brightness;
    }
}

int FPGARendererRPISPI::getGlobalBrightness() {
    return globalBrightness;
}
