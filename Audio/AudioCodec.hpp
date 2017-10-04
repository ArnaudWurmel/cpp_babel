#ifndef AUDIOCODEC_HPP_
#define AUDIOCODEC_HPP_

#include "IAudioCodec.hpp"
#include <iostream>
#include <opus/opus.h>
#include <string.h>

class OpusCodec : public IAudioCodec {
private:
  OpusEncoder *enc;
  OpusDecoder *dec;
  int FrameSize;
  int num_channels;
  int error;
  int data_size;
  int ret;

public:
  DecodedFrame AudioDecode(EncodedFrame frame);
  EncodedFrame AudioEncode(DecodedFrame frame);
  OpusCodec();
  ~OpusCodec();
};

#endif /* AUDIOCODEC_HPP_*/
