#include "Cobs.h"

#include <boost/log/trivial.hpp>

///* Stuffs "length" bytes of data at the location pointed to by
// * "input", writing the output to the location pointed to by
// * "output". Returns the number of bytes written to "output".
// */
//size_t Cobs::encode(const char *inputBuffer, size_t length, char *outputBuffer) {
//    size_t read_index = 0;
//    size_t write_index = 1;
//    size_t code_index = 0;
//    uint8_t code = 1;
//
//    while (read_index < length) {
//        if (inputBuffer[read_index] == 0) {
//            outputBuffer[code_index] = code;
//            code = 1;
//            code_index = write_index++;
//            read_index++;
//        } else {
//            outputBuffer[write_index++] = inputBuffer[read_index++];
//            code++;
//            if (code == 0xFF) {
//                outputBuffer[code_index] = code;
//                code = 1;
//                code_index = write_index++;
//            }
//        }
//    }
//
//    outputBuffer[code_index] = code;
//    outputBuffer[write_index++] = 0; //add zero delimiter
//    return write_index;
//}
//
//
///* Unstuffs "length" bytes of data at the location pointed to by
// * "input", writing the output * to the location pointed to by
// * "output". Returns the number of bytes written to "output" if
// * "input" was successfully unstuffed, and 0 if there was an
// * error unstuffing "input".
// */
//size_t Cobs::decode(const char *inputBuffer, size_t length, char *outputBuffer) {
//    size_t read_index = 0;
//    size_t write_index = 0;
//    uint8_t code;
//    uint8_t i;
//
//    while (read_index < length) {
//        code = inputBuffer[read_index];
//
//        if (read_index + code > length && code != 1) {
//            return 0;
//        }
//
//        read_index++;
//
//        for (i = 1; i < code; i++) {
//            outputBuffer[write_index++] = inputBuffer[read_index++];
//        }
//        if (code != 0xFF && read_index != length) {
//            outputBuffer[write_index++] = '\0';
//        }
//    }
//
//    return write_index;
//}

Cobs::Cobs(int bufferSize) {
    internalStreamBuffer.reserve(bufferSize);
}

std::vector<std::string> Cobs::insertBytesAndReturnDecodedPackets(const uint8_t *inputData, size_t length) {
//    BOOST_LOG_TRIVIAL(trace) << "[Cobs] Insert data with length " << length;
    std::lock_guard<std::mutex> lock(internalStreamBufferLock);
    std::vector<std::string> result;
    internalStreamBuffer.reserve(internalStreamBuffer.size()+length);
    int zeroCounter = 0;
    for (int i = 0; i < length; i++) {
        if (inputData[i] != 0) {
            internalStreamBuffer.push_back(inputData[i]);
        } else {
//            BOOST_LOG_TRIVIAL(trace) << "[Cobs] Received 0 packet delimiter";
            zeroCounter++;
            internalStreamBuffer.push_back(0);
            if(internalStreamBuffer.size() > 2){
                result.push_back(decode(internalStreamBuffer));
                BOOST_LOG_TRIVIAL(trace) << "[Cobs] Received 0 packet delimiter & bufferlen > 2 : " << internalStreamBuffer.size();
            }

            internalStreamBuffer.clear();
        }
    }
    BOOST_LOG_TRIVIAL(trace) << "[Cobs] Received a total of " << zeroCounter << " zero-packet delimiters";
    return result;
}

const std::string Cobs::encode(std::string input) {
    std::string result;
    result.resize(COBS_ENCODE_DST_BUF_LEN_MAX(input.size())+100, 0x0);

    size_t write_index = 1;
    size_t code_index = 0;
    uint8_t code = 1;

    auto readIndex = input.begin();
    while(readIndex < input.end()) {
        if (*readIndex == 0) {
            result.at(code_index) = code;
            code = 1;
            code_index = write_index++;
            readIndex++;
        } else {
            result.at(write_index++) = *readIndex++;
            code++;
            if (code == 0xFF) {
                result.at(code_index) = code;
                code = 1;
                code_index = write_index++;
            }
        }
    }

    result.at(code_index) = code;
    result.resize(write_index+1);
//    result.push_back(0x00); //zero delimiter
    return result;
}

const std::string Cobs::decode(std::string input) {
    std::string result;
    result.reserve(COBS_DECODE_DST_BUF_LEN_MAX(input.size()));
    auto readIndex = input.begin();
    while(readIndex < input.end()) {
        uint8_t code = *readIndex++;
        for (auto i = 1; i < code; i++) {
            result.push_back(*readIndex++);
        }
        if (code != 0xFF && readIndex != input.end()) {
            result.push_back((char) 0);
        }
    }
    result.pop_back(); //remove last 0
    return result;
}
