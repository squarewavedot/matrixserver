#include "catch.hpp"
#include <Cobs.h>
#include <Screen.h>
#include <matrixserver.pb.h>

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

static std::string test0 = {0, 0, 0, 0};
static std::string test0_result = {0x01, 0x01, 0x01, 0x01, 0x01, 0x00};

static std::string test1 = {0, 2, 3, 4, 5};
static std::string test1_result = {0x01, 0x5, 2, 3, 4, 5, 0x00};

static std::string test2 = {1, 2, 3, 4, 5, 0};
static std::string test2_result = {0x6, 1, 2, 3, 4, 5, 0x01, 0x00};

static std::string test3 = {1, 2, 3, 4, 5, 0, 6, 7, 8, 9};
static std::string test3_result = {0x06, 1, 2, 3, 4, 5, 0x05, 6, 7, 8, 9, 0x0};

static std::string test4 = {1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 0};
static std::string test4_result = {0x6, 1, 2, 3, 4, 5, 0x5, 6, 7, 8, 9, 0x1, 0x0};

static const uint8_t test_string_5[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                                        23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                                        63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
                                        83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101,
                                        102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
                                        118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133,
                                        134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                                        150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
                                        166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
                                        182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
                                        198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
                                        214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
                                        230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245,
                                        246, 247, 248, 249, 250, 251, 252, 253};
static const uint8_t test_string_5_result[] = {0xFE, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                                               20, 21, 22,
                                               23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                                               41, 42,
                                               43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                                               61, 62,
                                               63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
                                               81, 82,
                                               83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
                                               101,
                                               102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
                                               116, 117,
                                               118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
                                               132, 133,
                                               134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147,
                                               148, 149,
                                               150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
                                               164, 165,
                                               166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
                                               180, 181,
                                               182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195,
                                               196, 197,
                                               198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
                                               212, 213,
                                               214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                                               228, 229,
                                               230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
                                               244, 245,
                                               246, 247, 248, 249, 250, 251, 252, 253, 0x00};
const std::string test5((const char *) test_string_5, sizeof(test_string_5));
const std::string test5_result((const char *) test_string_5_result, sizeof(test_string_5_result));

static const uint8_t test_string_6[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                                        23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                                        63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
                                        83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101,
                                        102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
                                        118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133,
                                        134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                                        150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
                                        166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
                                        182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
                                        198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
                                        214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
                                        230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245,
                                        246, 247, 248, 249, 250, 251, 252, 253, 254};
static const uint8_t test_string_6_result[] = {0xFF, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                                               20, 21, 22,
                                               23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                                               41, 42,
                                               43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                                               61, 62,
                                               63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
                                               81, 82,
                                               83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
                                               101,
                                               102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
                                               116, 117,
                                               118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
                                               132, 133,
                                               134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147,
                                               148, 149,
                                               150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
                                               164, 165,
                                               166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
                                               180, 181,
                                               182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195,
                                               196, 197,
                                               198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
                                               212, 213,
                                               214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                                               228, 229,
                                               230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
                                               244, 245,
                                               246, 247, 248, 249, 250, 251, 252, 253, 254, 0x01, 0x00};
const std::string test6((const char *) test_string_6, sizeof(test_string_6));
const std::string test6_result((const char *) test_string_6_result, sizeof(test_string_6_result));

static const uint8_t test_string_7[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                                        23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                                        63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
                                        83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101,
                                        102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
                                        118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133,
                                        134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                                        150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
                                        166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
                                        182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
                                        198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
                                        214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
                                        230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245,
                                        246, 247, 248, 249, 250, 251, 252, 253, 254, 255};
static const uint8_t test_string_7_result[] = {0xFF, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                                               20, 21, 22,
                                               23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                                               41, 42,
                                               43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                                               61, 62,
                                               63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
                                               81, 82,
                                               83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
                                               101,
                                               102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
                                               116, 117,
                                               118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
                                               132, 133,
                                               134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147,
                                               148, 149,
                                               150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
                                               164, 165,
                                               166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
                                               180, 181,
                                               182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195,
                                               196, 197,
                                               198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
                                               212, 213,
                                               214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                                               228, 229,
                                               230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
                                               244, 245,
                                               246, 247, 248, 249, 250, 251, 252, 253, 254, 0x2, 255, 0x0};
const std::string test7((const char *) test_string_7, sizeof(test_string_7));
const std::string test7_result((const char *) test_string_7_result, sizeof(test_string_7_result));


std::string getHexArrayString(const uint8_t *data, size_t length) {
    std::stringstream result;
    for (int i = 0; i < length; i++) {
        result << "0x" << std::hex << (int) data[i] << ", ";
    }
    return result.str();
}

std::string getHexArrayString(std::string input) {
    return getHexArrayString((const uint8_t *) input.data(), input.size());
}

TEST_CASE("cobs string encode/decode", "[cobs]") {
    REQUIRE(test1.compare(test1) == 0);
    REQUIRE(test1.compare(test1_result) != 0);
    auto encoded = Cobs::encode(test1);
    auto decoded = Cobs::decode(test1_result);
    INFO("encoded      " << getHexArrayString(encoded) << "\n" << "  test0_result " << getHexArrayString(test1_result));
    CHECK(encoded.compare(test1_result) == 0);
    INFO("decoded " << getHexArrayString(decoded) << "\n" << "  test1   " << getHexArrayString(test1));
    CHECK(decoded.compare(test1) == 0);
}

TEST_CASE("cobs stream decoder test 1", "[cobs]") {
    Cobs cobsTest(100000);
    SECTION("test string 0") {
        auto cobsResult = cobsTest.insertBytesAndReturnDecodedPackets((const uint8_t *) test0_result.data(),
                                                                      test0_result.size());
        REQUIRE(cobsResult.size() == 1);
        INFO("expected " << getHexArrayString(cobsResult.at(0)) <<
                         "\n" << "  got      " << getHexArrayString(test0));
        REQUIRE(test0.compare(cobsResult.at(0)) == 0);
    }
    SECTION("test string 1") {
        auto cobsResult = cobsTest.insertBytesAndReturnDecodedPackets((const uint8_t *) test1_result.data(),
                                                                      test1_result.size());
        REQUIRE(cobsResult.size() == 1);
        INFO("expected " << getHexArrayString(cobsResult.at(0)) <<
                         "\n" << "  got      " << getHexArrayString(test1));
        REQUIRE(test1.compare(cobsResult.at(0)) == 0);
    }
    SECTION("test string 2") {
        auto cobsResult = cobsTest.insertBytesAndReturnDecodedPackets((const uint8_t *) test2_result.data(),
                                                                      test2_result.size());
        REQUIRE(cobsResult.size() == 1);
        INFO("expected " << getHexArrayString(cobsResult.at(0)) <<
                         "\n" << "  got      " << getHexArrayString(test2));
        REQUIRE(test2.compare(cobsResult.at(0)) == 0);
    }
    SECTION("test string 3") {
        auto cobsResult = cobsTest.insertBytesAndReturnDecodedPackets((const uint8_t *) test3_result.data(),
                                                                      test3_result.size());
        REQUIRE(cobsResult.size() == 1);
        INFO("expected " << getHexArrayString(cobsResult.at(0)) <<
                         "\n" << "  got      " << getHexArrayString(test3));
        REQUIRE(test3.compare(cobsResult.at(0)) == 0);
    }
    SECTION("test string 4") {
        auto cobsResult = cobsTest.insertBytesAndReturnDecodedPackets((const uint8_t *) test4_result.data(),
                                                                      test4_result.size());
        REQUIRE(cobsResult.size() == 1);
        INFO("expected " << getHexArrayString(cobsResult.at(0)) <<
                         "\n" << "  got      " << getHexArrayString(test4));
        REQUIRE(test4.compare(cobsResult.at(0)) == 0);
    }
    SECTION("test string 5") {
        auto cobsResult = cobsTest.insertBytesAndReturnDecodedPackets((const uint8_t *) test5_result.data(),
                                                                      test5_result.size());
        REQUIRE(cobsResult.size() == 1);
        INFO("expected " << getHexArrayString(cobsResult.at(0)) <<
                         "\n" << "  got      " << getHexArrayString(test5));
        REQUIRE(test5.compare(cobsResult.at(0)) == 0);
    }
    SECTION("test string 6") {
        auto cobsResult = cobsTest.insertBytesAndReturnDecodedPackets((const uint8_t *) test6_result.data(),
                                                                      test6_result.size());
        REQUIRE(cobsResult.size() == 1);
        INFO("expected " << getHexArrayString(cobsResult.at(0)) <<
                         "\n" << "  got      " << getHexArrayString(test6));
        REQUIRE(test6.compare(cobsResult.at(0)) == 0);
    }
    SECTION("test string 7") {
        auto cobsResult = cobsTest.insertBytesAndReturnDecodedPackets((const uint8_t *) test7_result.data(),
                                                                      test7_result.size());
        REQUIRE(cobsResult.size() == 1);
        INFO("expected " << getHexArrayString(cobsResult.at(0)) <<
                         "\n" << "  got      " << getHexArrayString(test7));
        REQUIRE(test7.compare(cobsResult.at(0)) == 0);
    }
}

TEST_CASE("cobs string encode/decode #2", "[cobs]") {
    SECTION("string 0") {
        auto encoded = Cobs::encode(test0);
        auto decoded = Cobs::decode(encoded);
        INFO("expected " << getHexArrayString(test0) << "\n" << "  got      " << getHexArrayString(decoded));
        REQUIRE(encoded.size() == test0_result.size());
        REQUIRE(decoded.size() == test0.size());
        REQUIRE(decoded == test0);
    }
    SECTION("string 1") {
        auto encoded = Cobs::encode(test1);
        auto decoded = Cobs::decode(encoded);
        INFO("expected " << getHexArrayString(test1) << "\n" << "  got      " << getHexArrayString(decoded));
        REQUIRE(encoded.size() == test1_result.size());
        REQUIRE(decoded.size() == test1.size());
        REQUIRE(decoded == test1);
    }
    SECTION("string 2") {
        auto encoded = Cobs::encode(test2);
        auto decoded = Cobs::decode(encoded);
        INFO("expected " << getHexArrayString(test2) << "\n" << "  got      " << getHexArrayString(decoded));
        REQUIRE(encoded.size() == test2_result.size());
        REQUIRE(decoded.size() == test2.size());
        REQUIRE(decoded == test2);
    }
    SECTION("string 3") {
        auto encoded = Cobs::encode(test3);
        auto decoded = Cobs::decode(encoded);
        INFO("expected " << getHexArrayString(test3) << "\n" << "  got      " << getHexArrayString(decoded));
        REQUIRE(encoded.size() == test3_result.size());
        REQUIRE(decoded.size() == test3.size());
        REQUIRE(decoded == test3);
    }
    SECTION("string 4") {
        auto encoded = Cobs::encode(test4);
        auto decoded = Cobs::decode(encoded);
        INFO("expected " << getHexArrayString(test4) << "\n" << "  got      " << getHexArrayString(decoded));
        REQUIRE(encoded.size() == test4_result.size());
        REQUIRE(decoded.size() == test4.size());
        REQUIRE(decoded == test4);
    }
    SECTION("string 5") {
        auto encoded = Cobs::encode(test5);
        auto decoded = Cobs::decode(encoded);
        INFO("expected " << getHexArrayString(test5) << "\n" << "  got      " << getHexArrayString(decoded));
        REQUIRE(encoded.size() == test5_result.size());
        REQUIRE(decoded.size() == test5.size());
        REQUIRE(decoded == test5);
    }
    SECTION("string 6") {
        auto encoded = Cobs::encode(test6);
        auto decoded = Cobs::decode(encoded);
        INFO("original " << getHexArrayString(test6) << "\n" << "  decoded  " << getHexArrayString(decoded));
        INFO("original_enc " << getHexArrayString(test6_result) << "\n" << "  encoded      " << getHexArrayString(encoded));
        CHECK(encoded.size() == test6_result.size());
        CHECK(decoded.size() == test6.size());
        REQUIRE(decoded == test6);
    }
    SECTION("string 7") {
        auto encoded = Cobs::encode(test7);
        auto decoded = Cobs::decode(encoded);
        INFO("expected " << getHexArrayString(test7) << "\n" << "  got      " << getHexArrayString(decoded));
        CHECK(encoded.size() == test7_result.size());
        CHECK(decoded.size() == test7.size());
        REQUIRE(decoded == test7);
    }
    SECTION("string 6+6") {
        auto teststring(test6);
        auto teststring_result(test6_result);
        teststring.append(test6);
        teststring_result.append(test6_result);
        auto encoded = Cobs::encode(teststring);
        auto decoded = Cobs::decode(encoded);
        INFO("expected " << getHexArrayString(teststring) << "\n" << "  got      " << getHexArrayString(decoded));
//        CHECK(encoded.size() == teststring_result.size());
//        CHECK(decoded.size() == teststring.size());
        REQUIRE(decoded == teststring);
    }
    SECTION("string 7+7") {
        auto teststring(test7);
        auto teststring_result(test7_result);
        teststring.append(test7);
        teststring_result.append(test7_result);
        auto encoded = Cobs::encode(teststring);
        auto decoded = Cobs::decode(encoded);
        INFO("expected " << getHexArrayString(teststring) << "\n" << "  got      " << getHexArrayString(decoded));
//        CHECK(encoded.size() == teststring_result.size());
//        CHECK(decoded.size() == teststring.size());
        REQUIRE(decoded == teststring);
    }
}

TEST_CASE("cobs stream decoder test 2", "[cobs]") {
    Cobs cobsTest(100000);
    std::string teststring(test0_result);
    teststring.append(test1_result);
    teststring.append(test2_result);
    teststring.append(test3_result);
    teststring.append(test4_result);
    teststring.append(test5_result);
    teststring.append(test6_result);
    teststring.append(test7_result);

    auto cobsResult = cobsTest.insertBytesAndReturnDecodedPackets((const uint8_t *) teststring.data(),
                                                                  teststring.size());
    REQUIRE(cobsResult.size() == 8);
    INFO("expected " << getHexArrayString(test0) << "\n" << "  got      " << getHexArrayString(cobsResult.at(0)));
    INFO("expected " << getHexArrayString(test1) << "\n" << "  got      " << getHexArrayString(cobsResult.at(1)));
    INFO("expected " << getHexArrayString(test2) << "\n" << "  got      " << getHexArrayString(cobsResult.at(2)));
    INFO("expected " << getHexArrayString(test3) << "\n" << "  got      " << getHexArrayString(cobsResult.at(3)));
    INFO("expected " << getHexArrayString(test4) << "\n" << "  got      " << getHexArrayString(cobsResult.at(4)));
    INFO("expected " << getHexArrayString(test5) << "\n" << "  got      " << getHexArrayString(cobsResult.at(5)));
    INFO("expected " << getHexArrayString(test6) << "\n" << "  got      " << getHexArrayString(cobsResult.at(6)));
    INFO("expected " << getHexArrayString(test7) << "\n" << "  got      " << getHexArrayString(cobsResult.at(7)));
    REQUIRE(test0 == cobsResult.at(0));
    REQUIRE(test1 == cobsResult.at(1));
    REQUIRE(test2 == cobsResult.at(2));
    REQUIRE(test3 == cobsResult.at(3));
    REQUIRE(test4 == cobsResult.at(4));
    REQUIRE(test5 == cobsResult.at(5));
    REQUIRE(test6 == cobsResult.at(6));
    REQUIRE(test7 == cobsResult.at(7));
}

TEST_CASE("cobs stream decoder screenMessage test", "[cobs]") {
    Cobs cobsTest(100000);

    std::vector<std::shared_ptr<Screen>> screens;
    for (int i = 0; i < 6; i++) {
        screens.push_back(std::make_shared<Screen>(64, 64, i));
    }
    auto setScreenMessage = std::make_shared<matrixserver::MatrixServerMessage>();
    setScreenMessage->set_messagetype(matrixserver::setScreenFrame);
    for (auto &screen : screens) {
        auto screenData = setScreenMessage->add_screendata();
        screenData->set_screenid(screen->getScreenId());
        screenData->set_framedata((char *) screen->getScreenData().data(), screen->getScreenDataSize() * sizeof(Color));
        screenData->set_encoding(matrixserver::ScreenData_Encoding_rgb24bbp);
    }

    auto original = setScreenMessage->SerializeAsString();
    std::string directEncoded = Cobs::encode(original);
    std::cout << getHexArrayString(directEncoded) << std::endl;
    auto cobsResult = cobsTest.insertBytesAndReturnDecodedPackets((const uint8_t *) directEncoded.data(),
                                                                  directEncoded.size());
    auto directDecodingResult = Cobs::decode(directEncoded);
    REQUIRE(directDecodingResult.size() == original.size());
    REQUIRE(directDecodingResult == original);

    REQUIRE(cobsResult.size() == 1);
    REQUIRE(cobsResult.at(0).size() == original.size());
    //INFO("expected " << getHexArrayString(test0) << "\n" << "  got      " << getHexArrayString(cobsResult.at(0)));
    REQUIRE(original == cobsResult.at(0));
}

TEST_CASE("cobs stream decoder screenMessage benchmark", "[cobs]") {
    using namespace std::chrono;
    Cobs cobsTest(100000);

    std::vector<std::shared_ptr<Screen>> screens;
    for (int i = 0; i < 6; i++) {
        screens.push_back(std::make_shared<Screen>(64, 64, i));
    }
    auto setScreenMessage = std::make_shared<matrixserver::MatrixServerMessage>();
    setScreenMessage->set_messagetype(matrixserver::setScreenFrame);
    for (auto &screen : screens) {
        auto screenData = setScreenMessage->add_screendata();
        screenData->set_screenid(screen->getScreenId());
        screenData->set_framedata((char *) screen->getScreenData().data(), screen->getScreenDataSize() * sizeof(Color));
        screenData->set_encoding(matrixserver::ScreenData_Encoding_rgb24bbp);
    }


    //time meassurement block
    std::string original, directEncoded, directDecodingResult;
    const int repititions = 1000;
    auto msStart = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for(int i = 0; i < repititions; i++){
        original = setScreenMessage->SerializeAsString();
    }
    auto msStopwatch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - msStart;
    WARN("Total Time for " << repititions << " protobuff serializations: " << msStopwatch.count() << " ms");

    msStart = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for(int i = 0; i < repititions; i++){
        directEncoded = Cobs::encode(original);
    }
    msStopwatch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - msStart;
    WARN("Total Time for " << repititions << " direct COBS encodings: " << msStopwatch.count() << " ms");

    msStart = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for(int i = 0; i < repititions; i++){
        directDecodingResult = Cobs::decode(directEncoded);
    }
    msStopwatch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - msStart;
    WARN("Total Time for " << repititions << " direct COBS decodings: " << msStopwatch.count() << " ms");

    std::vector<std::string> cobsResult;
    msStart = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for(int i = 0; i < repititions; i++){
        cobsResult = cobsTest.insertBytesAndReturnDecodedPackets((const uint8_t *) directEncoded.data(), directEncoded.size());
    }
    msStopwatch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - msStart;
    WARN("Total Time for " << repititions << " stream COBS decodings: " << msStopwatch.count() << " ms");

    REQUIRE(directDecodingResult.size() == original.size());
    REQUIRE(directDecodingResult == original);

    REQUIRE(cobsResult.size() == 1);
    REQUIRE(cobsResult.at(0).size() == original.size());
    //INFO("expected " << getHexArrayString(test0) << "\n" << "  got      " << getHexArrayString(cobsResult.at(0)));
    REQUIRE(original == cobsResult.at(0));
}
