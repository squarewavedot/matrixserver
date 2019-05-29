#ifndef MATRIXSERVER_COBS_H
#define MATRIXSERVER_COBS_H

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <vector>
#include <mutex>

#define COBS_ENCODE_DST_BUF_LEN_MAX(SRC_LEN)            ((SRC_LEN) + (((SRC_LEN) + 253u)/254u))
#define COBS_DECODE_DST_BUF_LEN_MAX(SRC_LEN)            (((SRC_LEN) == 0) ? 0u : ((SRC_LEN) - 1u))

class Cobs {
public:
    Cobs(int bufferSize);
    std::vector<std::string> insertBytesAndReturnDecodedPackets(const uint8_t *inputData, size_t length);
    static const std::string decode(std::string input);
    static const std::string encode(const std::string input);
private:
    std::string internalStreamBuffer;
    std::mutex internalStreamBufferLock;
};


#endif //MATRIXSERVER_COBS_H
