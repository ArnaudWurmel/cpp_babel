#ifndef AUDIOPA_HPP
#define AUDIOPA_HPP_

#include "AudioSettings.h"
#include <iostream>
#include <portaudio.h>

class AudioPA {
private:
  PaStreamParameters inputParameters;
  PaStreamParameters outputParameters;
  int err;
  int state;
  SAMPLE *inputSamples;
  SAMPLE *outputSamples;

public:
  int callBackFunction(const void *&, void *&, unsigned long &,
                       const PaStreamCallbackTimeInfo *&,
                       PaStreamCallbackFlags &);
  PaStream *stream;
  AudioPA();
  ~AudioPA();
  SAMPLE *getAudioFrames();
  bool sendAudioFrames(float *data);
  bool startAudio();
  bool stopAudio();
};

#endif /* AUDIOPA_HPP_ */
