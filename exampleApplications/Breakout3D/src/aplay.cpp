#include "aplay.h"

Aplay::Aplay(){
  //initialise simple sound buffers
  char add1 = 0, add2 = 0, add3 = 0;
  std::vector<unsigned char> bufferData1, bufferData2, bufferData3;
  for (int i = 0; i < 4*1024; i++){
    if(i%40 == 0){
      if(add1 == 0) add1 = 255;
      else add1 = 0;
    }
    if(i%80 == 0){
      if(add2 == 0) add2 = 255;
      else add2 = 0;
    }
    if(i%120 == 0){
      if(add3 == 0) add3 = 255;
      else add3 = 0;
    }
    bufferData1.push_back(add1);
    bufferData2.push_back(add2);
    bufferData3.push_back(add3);
  }
  soundBufferData_.push_back(bufferData1);
  soundBufferData_.push_back(bufferData2);
  soundBufferData_.push_back(bufferData3);
}

void Aplay::playGameSound(unsigned int id){
  if(id < soundBufferData_.size()){
    boost::async( [=]{ return this->playSoundBuffer(soundBufferData_.at(id)); } );
  }
}

void Aplay::playSoundBuffer(std::vector<unsigned char> buffer){
  int err;
/*  snd_pcm_t *handle;
  if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
          printf("Playback open error: %s\n", snd_strerror(err));
          exit(EXIT_FAILURE);
  }
  if ((err = snd_pcm_set_params(handle,
                                SND_PCM_FORMAT_U8,
                                SND_PCM_ACCESS_RW_INTERLEAVED,
                                1,
                                40000,
                                1,
                                0)) < 0) {
          printf("Playback open error: %s\n", snd_strerror(err));
          exit(EXIT_FAILURE);
  }
  snd_pcm_writei(handle, buffer.data(), buffer.size());
  snd_pcm_close(handle);
*/
}
