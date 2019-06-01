#include "FPGARendererRPISPI.h"

#include <fcntl.h>                // Needed for SPI port
#include <sys/ioctl.h>            // Needed for SPI port
#include <linux/spi/spidev.h>     // Needed for SPI port
#include <errno.h>
#include <cstring>

static const char *device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 32000000;
static uint16_t delay;
int ret, fd;


int screenWidth, screenHeight, bitDepthInBytes, screenCount, llen;
unsigned char *cmd_buf;

int SpiWriteRead (int fd, unsigned char *data, unsigned int length) {
//    struct spi_ioc_transfer spi; /* Bibliotheksstruktur fuer Schreiben/Lesen */
//    int i, ret;                          /* Zaehler, Returnwert */
//
////    /* Wortlaenge abfragen */
////    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
////    if (ret < 0)
////    {
////        perror("Fehler Get Wortlaenge");
////        exit(1);
////    }
////
////    /* Datenrate abfragen */
////    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
////    if (ret < 0)
////    {
////        perror("Fehler Get Speed");
////        exit(1);
////    }
//
//    /* Daten uebergeben */
//
//    spi.tx_buf        = (unsigned long)(data); // transmit from "data"
//    spi.rx_buf        = (unsigned long)(recvdata); // receive into "data"
//    spi.len           = length;
//    spi.delay_usecs   = 10;
//    spi.speed_hz      = speed;
//    spi.bits_per_word = bits;
//    spi.cs_change     = true;
//
//    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &spi) ;
//    if(ret < 0)
//    {
//        std::cout << "Fehler beim Senden/Empfangen - ioctl " << errno << std::endl;
//        exit(1);
//    }
//    return ret;
    struct spi_ioc_transfer spi ;

// Mentioned in spidev.h but not used in the original kernel documentation
//	test program )-:

    memset (&spi, 0, sizeof (spi)) ;

    spi.tx_buf        = (unsigned long)data ;
    spi.rx_buf        = (unsigned long)data ;
    spi.len           = length ;
    spi.delay_usecs   = 0 ;
    spi.speed_hz      = speed;
    spi.bits_per_word = 8;

    return ioctl (fd, SPI_IOC_MESSAGE(1), &spi) ;
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
    llen = screenWidth * bitDepthInBytes * screenCount;
    cmd_buf = (unsigned char*)malloc(llen+128);


    std::cout << "Init SPI Driver" << std::endl;

//    spidevfilehandle = wiringPiSPISetup(0, speed);

//    gpioInitialise();
//    uint32_t spiSpeed = 30000000;
//    uint32_t spiFlags = 0x00 & 0x3FFFFF; //http://abyz.me.uk/rpi/pigpio/cif.html#spiOpen
//    spidevfilehandle = spiOpen(0,spiSpeed,spiFlags);
//
/* Device oeffen */
    if ((spidevfilehandle = open(device, O_RDWR)) < 0)
    {
        perror("Fehler Open Device");
        exit(1);
    }
/* Mode setzen */
    ret = ioctl(spidevfilehandle, SPI_IOC_WR_MODE, &mode);
    if (ret < 0)
    {
        perror("Fehler Set SPI-Modus");
        exit(1);
    }

/* Mode abfragen */
    ret = ioctl(spidevfilehandle, SPI_IOC_RD_MODE, &mode);
    if (ret < 0)
    {
        perror("Fehler Get SPI-Modus");
        exit(1);
    }

/* Wortlaenge setzen */
    ret = ioctl(spidevfilehandle, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret < 0)
    {
        perror("Fehler Set Wortlaenge");
        exit(1);
    }

/* Wortlaenge abfragen */
    ret = ioctl(spidevfilehandle, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret < 0)
    {
        perror("Fehler Get Wortlaenge");
        exit(1);
    }

/* Datenrate setzen */
    ret = ioctl(spidevfilehandle, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret < 0)
    {
        perror("Fehler Set Speed");
        exit(1);
    }

/* Datenrate abfragen */
    ret = ioctl(spidevfilehandle, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret < 0)
    {
        perror("Fehler Get Speed");
        exit(1);
    }

/* Kontrollausgabe */
    printf("SPI-Device.....: %s\n", device);
    printf("SPI-Mode.......: %d\n", mode);
    printf("Wortlaenge.....: %d\n", bits);
    printf("Geschwindigkeit: %d Hz (%d MHz)\n", speed, speed/1000000);

}

void FPGARendererRPISPI::setScreenData(int screenId, Color *screenData) {
    if(!renderMutex.try_lock())
        return;
    if (screenId < screens.size()) {
        screens.at(screenId)->setScreenData(screenData);
    }
    renderMutex.unlock();
}

void FPGARendererRPISPI::render() {
    if(!renderMutex.try_lock())
        return;
    auto usStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());

    /* Doing VSync first */
#if 1
    char recv_buf[2];
    do {
        cmd_buf[0] = 0x00;
        cmd_buf[1] = 0x00;
//        std::cout << "vsync" << std::endl;
        SpiWriteRead(spidevfilehandle, cmd_buf, 2);
//        spiXfer(spidevfilehandle, cmd_buf, recv_buf, 2);
//        wiringPiSPIDataRW(0, cmd_buf, 2);
//        printf("%d\n", recv_buf[0] | recv_buf[1]);
    } while (((cmd_buf[0] | cmd_buf[1]) & 0x02) != 0x02);
#endif

//
////    auto usTotal2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()) - usStart;
////    std::cout << "vsync:  " << usTotal2.count() << " us" << std::endl;

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
                pixelP = (uint16_t *)(cmd_buf + i + screen->getOffsetX() * (llen / screenCount) + x * bitDepthInBytes);
//                *pixelP = (tmpColor.r() | tmpColor.g()<<6 | tmpColor.b()<<11);
                *pixelP = tmpColor.r() >> 3 << 11 | tmpColor.g() >> 2 << 6 | tmpColor.b() >> 3;
                //bitDepthInBytes == 3
//                cmd_buf[i + screen->getOffsetX() * (llen / screenCount) + x * bitDepthInBytes] = tmpColor.r();
//                cmd_buf[i + screen->getOffsetX() * (llen / screenCount) + x * bitDepthInBytes + 1] = tmpColor.g();
//                cmd_buf[i + screen->getOffsetX() * (llen / screenCount) + x * bitDepthInBytes + 2] = tmpColor.b();
            }
        }
        i += llen;

//        spiWrite(spidevfilehandle, cmd_buf, i);
//        wiringPiSPIDataRW(0, cmd_buf, i);

//        std::cout << "line" << std::endl;
        SpiWriteRead(spidevfilehandle, cmd_buf, i);

        cmd_buf[0] = 0x03;
        cmd_buf[1] = y;

//        spiWrite(spidevfilehandle, cmd_buf, 2);
//        wiringPiSPIDataRW(0, cmd_buf, 2);
//        std::cout << "line swap" << std::endl;
        SpiWriteRead(spidevfilehandle, cmd_buf, 2);
    }

    /* Swap Frame */
    cmd_buf[0] = 0x04;
    cmd_buf[1] = 0x00;

//    wiringPiSPIDataRW(0, cmd_buf, 2);
//    spiWrite(spidevfilehandle, cmd_buf, 2);
//    std::cout << "frameswap" << std::endl;
    SpiWriteRead(spidevfilehandle, cmd_buf, 2);



    auto usTotal = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()) - usStart;
    std::cout << "render: " << usTotal.count() << " us" << std::endl;

    renderMutex.unlock();
}

void FPGARendererRPISPI::setGlobalBrightness(int brightness) {
    if (brightness <= 100 && brightness >= 0) {
        globalBrightness = brightness;
    }
}

int FPGARendererRPISPI::getGlobalBrightness() {
    return globalBrightness;
}
