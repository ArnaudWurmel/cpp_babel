#ifndef PLAY_HPP_
#define PLAY_HPP_

#include "AudioSettings.h"
#include "IAudio.hpp"
#include <iostream>
#include <list>
#include <portaudio.h>

class Play : public IAudio {
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
  bool startAudio();
  bool stopAudio();
};
#endif /* RECORD_HPP_ */
