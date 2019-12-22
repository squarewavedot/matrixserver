#include <cmath>
#include "Color.h"

Color::Color() {
    Color(0);
}

Color::Color(uint8_t brightness) {
    Color(brightness, brightness, brightness);
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue) {
    r_ = red;
    g_ = green;
    b_ = blue;
}

void Color::r(unsigned int red) {
    r_ = (red <= UINT8_MAX) ? red : UINT8_MAX;
}

void Color::g(unsigned int green) {
    g_ = (green <= UINT8_MAX) ? green : UINT8_MAX;
}

void Color::b(unsigned int blue) {
    b_ = (blue <= UINT8_MAX) ? blue : UINT8_MAX;
}

void Color::r(uint8_t red) {
    r_ = red;
}

void Color::g(uint8_t green) {
    g_ = green;
}

void Color::b(uint8_t blue) {
    b_ = blue;
}

uint8_t Color::r() const {
    return r_;
}

uint8_t Color::g() const {
    return g_;
}

uint8_t Color::b() const {
    return b_;
}

void Color::fromHSV(float h, float s, float v) {
    int i;
    float f, p, q, t;
    if (s == 0) {
        // achromatic (grey)
        r_ = g_ = b_ = v * UINT8_MAX;
        return;
    }
    h /= 60;            // sector 0 to 5
    i = floor(h);
    f = h - i;            // factorial part of h
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    switch (i) {
        case 0:
            r_ = v * UINT8_MAX;
            g_ = t * UINT8_MAX;
            b_ = p * UINT8_MAX;
            break;
        case 1:
            r_ = q * UINT8_MAX;
            g_ = v * UINT8_MAX;
            b_ = p * UINT8_MAX;
            break;
        case 2:
            r_ = p * UINT8_MAX;
            g_ = v * UINT8_MAX;
            b_ = t * UINT8_MAX;
            break;
        case 3:
            r_ = p * UINT8_MAX;
            g_ = q * UINT8_MAX;
            b_ = v * UINT8_MAX;
            break;
        case 4:
            r_ = t * UINT8_MAX;
            g_ = p * UINT8_MAX;
            b_ = v * UINT8_MAX;
            break;
        default:        // case 5:
            r_ = v * UINT8_MAX;
            g_ = p * UINT8_MAX;
            b_ = q * UINT8_MAX;
            break;
    }
}

Color &Color::operator+=(const Color &rhs) {
    r_ = (r_ + rhs.r() < UINT8_MAX) ? r_ + rhs.r() : UINT8_MAX;
    g_ = (g_ + rhs.g() < UINT8_MAX) ? g_ + rhs.g() : UINT8_MAX;
    b_ = (b_ + rhs.b() < UINT8_MAX) ? b_ + rhs.b() : UINT8_MAX;
    return *this;
}

Color operator+(Color lhs, const Color &rhs) {
    lhs += rhs;
    return lhs;
}

Color &Color::operator-=(const Color &rhs) {
    r_ = (r_ - rhs.r() > 0) ? r_ - rhs.r() : 0;
    g_ = (g_ - rhs.g() > 0) ? g_ - rhs.g() : 0;
    b_ = (b_ - rhs.b() > 0) ? b_ - rhs.b() : 0;
    return *this;
}

Color operator-(Color lhs, const Color &rhs) {
    lhs -= rhs;
    return lhs;
}

Color &Color::operator*=(const float &rhs) {
    int tempRed = r_ * rhs;
    int tempGreen = g_ * rhs;
    int tempBlue = b_ * rhs;
    r_ = (tempRed > UINT8_MAX) ? UINT8_MAX : (tempRed < 0) ? 0 : tempRed;
    g_ = (tempGreen > UINT8_MAX) ? UINT8_MAX : (tempGreen < 0) ? 0 : tempGreen;
    b_ = (tempBlue > UINT8_MAX) ? UINT8_MAX : (tempRed < 0) ? 0 : tempBlue;
    return *this;
}

Color operator*(Color lhs, const float &rhs) {
    lhs *= rhs;
    return lhs;
}

bool operator==(const Color &lhs, const Color &rhs) {
    return lhs.r() == rhs.r() && lhs.g() == rhs.g() && lhs.b() == rhs.b();
}

bool operator!=(const Color &lhs, const Color &rhs) {
    return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &out, const Color &o) {
    return out << "RGB(" << (int) o.r() << "," << (int) o.g() << "," << (int) o.b() << ")";
}
