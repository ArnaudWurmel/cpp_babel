#ifndef AUDIOCODEC_HPP_
#define AUDIOCODEC_HPP_

#include <opus/opus.h>
#include <string.h>
#include <iostream>

class AudioCodec {
private:
  OpusEncoder *enc;
  OpusDecoder *dec;
  int FrameSize;
  int num_channels;
  int error;
  int data_size;
    int ret;

public:
  float *AudioDecode(unsigned char *data);
  unsigned char *AudioEncode(float *frame);
  AudioCodec();
  ~AudioCodec();
};

#endif /* AUDIOCODEC_HPP_*/
