#ifndef PLAY_HPP_
#define PLAY_HPP_

#include "AudioSettings.h"
#include <iostream>
#include <list>
#include <portaudio.h>

class Play {
private:
  PaStreamParameters outputParameters;
  int state;
  int err;
  SAMPLE *outputSamples;

public:
  int callBackFunction(const void *&, void *&, unsigned long &,
                       const PaStreamCallbackTimeInfo *&,
                       PaStreamCallbackFlags &);
  PaStream *stream;
  std::list<DecodedFrame> Fbuffer;
  Play();
  ~Play();
  bool PlayFrames(DecodedFrame);
  bool startPlay();
  bool stopPlay();
};
#endif /* RECORD_HPP_ */
