#include "AudioCodec.hpp"
#include "AudioPA.hpp"
#include <iostream>

int main() {
  AudioPA *audio = new AudioPA;
  AudioCodec *codec = new AudioCodec;
  int i = 0;
  unsigned char *buffer = NULL;
  float *frame;
  audio->startAudio();
  while (Pa_IsStreamActive(audio->stream) == 1) {
    i++;
    buffer = codec->AudioEncode(audio->getAudioFrames());
    frame = codec->AudioDecode(buffer);
    audio->sendAudioFrames(frame);
    delete buffer;
    delete frame;
  }
}
