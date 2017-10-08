#ifndef IAUDIOCODEC_HPP_
#define IAUDIOCODEC_HPP_

#include "AudioSettings.h"

class IAudioCodec {

public:
    virtual ~IAudioCodec() {}
  virtual DecodedFrame AudioDecode(EncodedFrame) = 0;
  virtual EncodedFrame AudioEncode(DecodedFrame) = 0;
};

#endif /* IAUDIOCODEC_HPP_ */
