#ifndef RECORD_HPP_
#define RECORD_HPP_

#include "AudioSettings.h"
#include <iostream>
#include <list>
#include <portaudio.h>

class Record {
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
  bool startRecord();
  bool stopRecord();
};
#endif /* RECORD_HPP_ */
