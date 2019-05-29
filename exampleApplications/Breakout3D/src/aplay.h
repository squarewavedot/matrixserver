#ifndef __APLAY_H__
#define __APLAY_H__

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/thread/future.hpp>

class Aplay{
public:
  Aplay();
  void playGameSound(unsigned int id);
private:
  void playSoundBuffer(std::vector<unsigned char> buffer);
  boost::thread * thread_;
  std::vector<std::vector<unsigned char>> soundBufferData_;
  std::vector<unsigned char> bufferData1_;
  std::vector<unsigned char> bufferData2_;
  std::vector<unsigned char> bufferData3_;
  // std::vector<unsigned char[2*1024]> sounds_;
};

#endif
