#ifndef RECORD_HPP_
#define RECORD_HPP_

#include "AudioSettings.h"
#include "IAudio.hpp"
#include <iostream>
#include <list>
#include <portaudio.h>

class Record : public IAudio {
private:
  PaStreamParameters inputParameters;
  int state;
  int err;
  SAMPLE *inputSamples;

public:
  int callBackFunction(const void *&, void *&, unsigned long &,
                       const PaStreamCallbackTimeInfo *&,
                       PaStreamCallbackFlags &);

  PaStream *stream;
  std::list<DecodedFrame> Fbuffer;
  Record();
  ~Record();
  DecodedFrame RecordedFrames();
  bool startAudio();
  bool stopAudio();
    bool isActive() const;
};
#endif /* RECORD_HPP_ */
