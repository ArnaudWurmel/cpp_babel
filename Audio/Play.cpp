#include "Play.hpp"

Play::Play() {
  if ((this->err = Pa_Initialize()) != paNoError)
    std::cout << "Error : " << Pa_GetErrorText(err) << '\n';
  this->outputParameters.device = Pa_GetDefaultOutputDevice();
  this->outputParameters.channelCount = 2;
  this->outputParameters.sampleFormat = paFloat32;
  this->outputParameters.suggestedLatency =
      Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  this->outputParameters.hostApiSpecificStreamInfo = NULL;
  this->outputSamples = NULL;
}

Play::~Play() {
  if ((this->err = Pa_IsStreamActive(this->stream)) == 1)
    Pa_CloseStream(this->stream);
  Pa_Terminate();
}

bool Play::PlayFrames(DecodedFrame frame) {
  if (frame.size > 0)
    this->Fbuffer.push_back(frame);
  return (true);
}

int Play::callBackFunction(const void *&inputBuffer, void *&outputBuffer,
                           unsigned long &framesPerBuffer,
                           const PaStreamCallbackTimeInfo *&timeInfo,
                           PaStreamCallbackFlags &statusFlags) {

  DecodedFrame frame;
  SAMPLE *wptr = (SAMPLE *)outputBuffer;
  int i = 0;

  if (this->Fbuffer.size() == 0) {
    wptr = NULL;
    return (paContinue);
  }
  frame = this->Fbuffer.front();
  while (i < frame.size) {
    *wptr++ = frame.frame[i];
    i++;
  }
  this->Fbuffer.pop_front();
  if (this->state == PA_ON)
    return paContinue;
  else
    return paComplete;
}

static int Pa_callBack(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags, void *userData) {
  Play *play = static_cast<Play *>(userData);
  return play->callBackFunction(inputBuffer, outputBuffer, framesPerBuffer,
                                timeInfo, statusFlags);
}

bool Play::startAudio() {
  Pa_OpenStream(&this->stream, NULL, &this->outputParameters, SAMPLE_RATE,
                FRAMES_PER_BUFFER, paClipOff, Pa_callBack, this);
  Pa_StartStream(this->stream);
  this->state = PA_ON;
  return (true);
}

bool Play::stopAudio() {
  this->state = PA_OFF;
  Pa_CloseStream(this->stream);
  return (true);
}
