#ifndef __FONT6PX_H__
#define __FONT6PX_H__

#include <vector>
#include <string>
#include <Eigen/Dense>
#include <Eigen/StdVector>
using namespace Eigen;

typedef std::vector<Vector2i> Bitmap1bpp;

namespace CharacterBitmaps
{
  static const int fontWidth = 4;
  static const int fontHeight = 6;
  enum FontAlignment { left = 0, centered = -1, right = -2 };

  const Bitmap1bpp char_0 = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(1,4),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_1 = {
    Vector2i(1,0),
    Vector2i(1,1),
    Vector2i(1,2),
    Vector2i(1,3),
    Vector2i(1,4),
  };

  const Bitmap1bpp char_2 = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(2,1),
    Vector2i(2,2),
    Vector2i(1,2),
    Vector2i(0,2),
    Vector2i(0,3),
    Vector2i(0,4),
    Vector2i(1,4),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_3 = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(2,1),
    Vector2i(2,2),
    Vector2i(1,2),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(1,4),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_4 = {
    Vector2i(0,0),
    Vector2i(0,1),
    Vector2i(0,2),
    Vector2i(1,2),
    Vector2i(2,0),
    Vector2i(2,1),
    Vector2i(2,2),
    Vector2i(2,3),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_5 = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,2),
    Vector2i(1,2),
    Vector2i(0,2),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(1,4),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_6 = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(1,2),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(1,4),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_7 = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(2,1),
    Vector2i(2,2),
    Vector2i(2,3),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_8 = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(1,4),
    Vector2i(2,4),
    Vector2i(1,2),
  };

  const Bitmap1bpp char_9 = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(1,4),
    Vector2i(2,4),
    Vector2i(1,2),
  };

  const Bitmap1bpp char_A = {
    Vector2i(1,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(2,4),
    Vector2i(1,2),
  };

  const Bitmap1bpp char_B = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(1,4),
    Vector2i(1,2),
  };

  const Bitmap1bpp char_C = {
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(0,2),
    Vector2i(0,3),
    Vector2i(1,4),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_D = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(1,4),
  };

  const Bitmap1bpp char_E = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(0,4),
    Vector2i(1,4),
    Vector2i(2,4),
    Vector2i(1,2),
  };

  const Bitmap1bpp char_F = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(0,4),
    Vector2i(1,2),
  };

  const Bitmap1bpp char_G = {
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(1,2),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(1,4),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_H = {
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(2,4),
    Vector2i(1,2),
  };

  const Bitmap1bpp char_I = {
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(1,0),
    Vector2i(1,1),
    Vector2i(1,2),
    Vector2i(1,3),
    Vector2i(1,4),
    Vector2i(0,4),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_J = {
    Vector2i(2,0),
    Vector2i(2,1),
    Vector2i(2,2),
    Vector2i(2,3),
    Vector2i(1,4),
    Vector2i(0,4),
  };

  const Bitmap1bpp char_K = {
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(2,4),
    Vector2i(1,2),
  };

  const Bitmap1bpp char_L = {
    Vector2i(0,0),
    Vector2i(0,1),
    Vector2i(0,2),
    Vector2i(0,3),
    Vector2i(0,4),
    Vector2i(1,4),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_M = {
    Vector2i(1,1),
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(2,4),
    Vector2i(1,2),
  };

  const Bitmap1bpp char_N = {
    Vector2i(1,1),
    Vector2i(1,3),
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(2,4),
    Vector2i(1,2),
  };

  const Bitmap1bpp char_O = {
    Vector2i(1,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(1,4),
  };

  const Bitmap1bpp char_P = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(0,3),
    Vector2i(0,4),
    Vector2i(1,2),
  };

  const Bitmap1bpp char_Q = {
    Vector2i(1,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(1,4),
    Vector2i(2,4),
    Vector2i(1,3),
  };

  const Bitmap1bpp char_R = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(0,3),
    Vector2i(0,4),
    Vector2i(1,2),
    Vector2i(1,3),
    Vector2i(2,4),
    Vector2i(2,2),
  };

  const Bitmap1bpp char_S = {
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(1,2),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(1,4),
  };

  const Bitmap1bpp char_T = {
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(1,0),
    Vector2i(1,1),
    Vector2i(1,2),
    Vector2i(1,3),
    Vector2i(1,4),
  };

  const Bitmap1bpp char_U = {
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(1,4),
    Vector2i(2,4),
  };

  const Bitmap1bpp char_V = {
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(1,3),
    Vector2i(1,4),
  };

  const Bitmap1bpp char_W = {
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(2,4),
    Vector2i(1,2),
    Vector2i(1,3),
  };

  const Bitmap1bpp char_X = {
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(0,3),
    Vector2i(2,3),
    Vector2i(0,4),
    Vector2i(2,4),
    Vector2i(1,2),
    Vector2i(1,3),
  };

  const Bitmap1bpp char_Y = {
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(0,1),
    Vector2i(2,1),
    Vector2i(1,2),
    Vector2i(1,3),
    Vector2i(1,4),
  };

  const Bitmap1bpp char_Z = {
    Vector2i(0,0),
    Vector2i(1,0),
    Vector2i(2,0),
    Vector2i(0,4),
    Vector2i(1,4),
    Vector2i(2,4),
    Vector2i(2,1),
    Vector2i(1,2),
    Vector2i(0,3),
  };

  const Bitmap1bpp char_colon = {
    Vector2i(1,1),
    Vector2i(1,3),
  };

  const Bitmap1bpp char_dot = {
    Vector2i(1,4),
  };

  const Bitmap1bpp char_comma = {
    Vector2i(1,3),
    Vector2i(0,4),
  };

  const Bitmap1bpp char_matrix = {
    Vector2i(0,0),
    Vector2i(2,0),
    Vector2i(1,1),
    Vector2i(3,1),
    Vector2i(0,2),
    Vector2i(2,2),
    Vector2i(1,3),
    Vector2i(3,3),
    Vector2i(0,4),
    Vector2i(2,4),
    Vector2i(1,5),
    Vector2i(3,5),

  };



  inline int getStringWidth(std::string str){
    return str.length() * fontWidth;
  }

  inline Bitmap1bpp getBitmapFromChar(char character){
    Bitmap1bpp bitmap = CharacterBitmaps::char_matrix;
    switch (character) {
      case 'A':
      case 'a':
        bitmap = CharacterBitmaps::char_A;
      break;
      case 'B':
      case 'b':
        bitmap = CharacterBitmaps::char_B;
      break;
      case 'C':
      case 'c':
        bitmap = CharacterBitmaps::char_C;
      break;
      case 'D':
      case 'd':
        bitmap = CharacterBitmaps::char_D;
      break;
      case 'E':
      case 'e':
        bitmap = CharacterBitmaps::char_E;
      break;
      case 'F':
      case 'f':
        bitmap = CharacterBitmaps::char_F;
      break;
      case 'G':
      case 'g':
        bitmap = CharacterBitmaps::char_G;
      break;
      case 'H':
      case 'h':
        bitmap = CharacterBitmaps::char_H;
      break;
      case 'I':
      case 'i':
        bitmap = CharacterBitmaps::char_I;
      break;
      case 'J':
      case 'j':
        bitmap = CharacterBitmaps::char_J;
      break;
      case 'K':
      case 'k':
        bitmap = CharacterBitmaps::char_K;
      break;
      case 'L':
      case 'l':
        bitmap = CharacterBitmaps::char_L;
      break;
      case 'M':
      case 'm':
        bitmap = CharacterBitmaps::char_M;
      break;
      case 'N':
      case 'n':
        bitmap = CharacterBitmaps::char_N;
      break;
      case 'O':
      case 'o':
        bitmap = CharacterBitmaps::char_O;
      break;
      case 'P':
      case 'p':
        bitmap = CharacterBitmaps::char_P;
      break;
      case 'Q':
      case 'q':
        bitmap = CharacterBitmaps::char_Q;
      break;
      case 'R':
      case 'r':
        bitmap = CharacterBitmaps::char_R;
      break;
      case 'S':
      case 's':
        bitmap = CharacterBitmaps::char_S;
      break;
      case 'T':
      case 't':
        bitmap = CharacterBitmaps::char_T;
      break;
      case 'U':
      case 'u':
        bitmap = CharacterBitmaps::char_U;
      break;
      case 'V':
      case 'v':
        bitmap = CharacterBitmaps::char_V;
      break;
      case 'W':
      case 'w':
        bitmap = CharacterBitmaps::char_W;
      break;
      case 'X':
      case 'x':
        bitmap = CharacterBitmaps::char_X;
      break;
      case 'Y':
      case 'y':
        bitmap = CharacterBitmaps::char_Y;
      break;
      case 'Z':
      case 'z':
        bitmap = CharacterBitmaps::char_Z;
      break;
      case '0':
        bitmap = CharacterBitmaps::char_0;
      break;
      case '1':
        bitmap = CharacterBitmaps::char_1;
      break;
      case '2':
        bitmap = CharacterBitmaps::char_2;
      break;
      case '3':
        bitmap = CharacterBitmaps::char_3;
      break;
      case '4':
        bitmap = CharacterBitmaps::char_4;
      break;
      case '5':
        bitmap = CharacterBitmaps::char_5;
      break;
      case '6':
        bitmap = CharacterBitmaps::char_6;
      break;
      case '7':
        bitmap = CharacterBitmaps::char_7;
      break;
      case '8':
        bitmap = CharacterBitmaps::char_8;
      break;
      case '9':
        bitmap = CharacterBitmaps::char_9;
      break;
      case ':':
        bitmap = CharacterBitmaps::char_colon;
      break;
      case '.':
        bitmap = CharacterBitmaps::char_dot;
      break;
      case ',':
        bitmap = CharacterBitmaps::char_comma;
      break;
      case ' ':
        bitmap.clear();
      break;
    }
    return bitmap;
  }
} //end namespace CharacterBitmap1bpps

#endif
