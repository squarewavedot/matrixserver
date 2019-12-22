#ifndef __COLOR_HH__
#define __COLOR_HH__

#include <stdint.h>
#include <iostream>

class Color {
  public:
    Color();
    Color(uint8_t brightness);
    Color(uint8_t red, uint8_t green, uint8_t blue);
    void r(unsigned int red);
    void g(unsigned int green);
    void b(unsigned int blue);
    void r(uint8_t red);
    void g(uint8_t green);
    void b(uint8_t blue);
    uint8_t r() const;
    uint8_t g() const;
    uint8_t b() const;
    void fromHSV(float h, float s, float v);
    Color& operator+=(const Color& rhs);
    friend Color operator+(Color lhs,const Color& rhs);
    Color& operator-=(const Color& rhs);
    friend Color operator-(Color lhs,const Color& rhs);
    Color& operator*=(const float& rhs);
    friend Color operator*(Color lhs,const float& rhs);
    friend bool operator==(const Color& lhs, const Color& rhs);
    friend bool operator!=(const Color& lhs, const Color& rhs);
    friend std::ostream& operator<<(std::ostream& out, const Color& o);
    static Color black(){ return Color(0,0,0); };
    static Color white(){ return Color(255,255,255); };
    static Color red(){   return Color(255,0,0); };
    static Color green(){ return Color(0,255,0); };
    static Color blue(){  return Color(0,0,255); };
    static Color randomRed(){  return Color(rand()%255,0,0); };
    static Color randomGreen(){  return Color(0,rand()%255,0); };
    static Color randomBlue(){  return Color(0,0,rand()%255); };
    static Color random(){  return Color(rand()%255,rand()%255,rand()%255); };
  private:
    uint8_t r_;
    uint8_t g_;
    uint8_t b_;
};

#endif
