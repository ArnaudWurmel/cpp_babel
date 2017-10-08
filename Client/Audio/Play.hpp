#ifndef PLAY_HPP_
#define PLAY_HPP_

#include "AudioSettings.h"
#include "IAudio.hpp"
#include <iostream>
#include <list>
#include <mutex>
#include "portaudio.h"
#include "IPlay.h"

class Play : public IPlay {
private:
  PaStreamParameters outputParameters;
  int state;
  int err;
    std::mutex  _lock;
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
    bool    isActive() const;
};
#endif /* RECORD_HPP_ */
