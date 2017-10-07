#include "Play.hpp"

Play::Play() {
	this->stream = NULL;
  if ((this->err = Pa_Initialize()) != paNoError)
    std::cout << "Error : " << Pa_GetErrorText(err) << '\n';
  this->outputParameters.device = Pa_GetDefaultOutputDevice();
  if (this->outputParameters.device == paNoDevice)
    return ;
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
}

bool Play::PlayFrames(DecodedFrame frame) {
  _lock.lock();
  if (frame.size > 0)
    this->Fbuffer.push_back(frame);
  _lock.unlock();
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
  _lock.lock();
  this->Fbuffer.pop_front();
  _lock.unlock();
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
	if (Pa_OpenStream(&this->stream, NULL, &this->outputParameters, SAMPLE_RATE,
		FRAMES_PER_BUFFER, paClipOff, Pa_callBack, this) == paNoError && Pa_StartStream(this->stream) == paNoError) {
		this->state = PA_ON;
		return (true);
	}
	this->stream = NULL;
	return (false);
}

bool Play::stopAudio() {
  this->state = PA_OFF;
  Pa_CloseStream(this->stream);
  return (true);
}

bool  Play::isActive() const {
    return Pa_IsStreamActive(this->stream) == 1;
}
