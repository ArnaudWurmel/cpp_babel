#include "portaudio.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

typedef float SAMPLE;
#define SAMPLE_RATE (44100)
#define NUM_CHANNELS (2)

static int callBack(const void *inputBuffer, void *outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo *timeInfo,
                    PaStreamCallbackFlags statusFlags, void *userData) {
  const SAMPLE *rptr = (const SAMPLE *)inputBuffer;
  SAMPLE *wptr = (SAMPLE *)outputBuffer;
  unsigned long i;
  if (inputBuffer == NULL) {
    for (i = 0; i < framesPerBuffer; i++) {
      *wptr++ = 0;
      if (NUM_CHANNELS == 2)
        *wptr++ = 0;
    }
  } else {
    for (i = 0; i < framesPerBuffer; i++) {
      *wptr++ = *rptr++;
      if (NUM_CHANNELS == 2)
        *wptr++ = *rptr++;
    }
  }
  return paContinue;
}

int main(void) {
  PaStreamParameters inputParameters, outputParameters;
  PaStream *stream;
  int err;

  if ((err = Pa_Initialize()) != paNoError) {
    std::cout << "Error :" << Pa_GetErrorText(err) << std::endl;
    return (1);
  }

  inputParameters.device = Pa_GetDefaultInputDevice();
  inputParameters.channelCount = 2;
  inputParameters.sampleFormat = paFloat32;
  inputParameters.suggestedLatency =
      Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;
  outputParameters.device = Pa_GetDefaultOutputDevice();
  outputParameters.channelCount = 2;
  outputParameters.sampleFormat = paFloat32;
  outputParameters.suggestedLatency =
      Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;
  Pa_OpenStream(&stream, &inputParameters, &outputParameters, SAMPLE_RATE, 512,
                paClipOff, callBack, NULL);
  Pa_StartStream(stream);
  while ((err = Pa_IsStreamActive(stream)) == 1) {
    Pa_Sleep(1000);
    std::cout << "Recording ! Speak into the microhpone" << std::endl;
  }
  Pa_CloseStream(stream);
  Pa_Terminate();
}
