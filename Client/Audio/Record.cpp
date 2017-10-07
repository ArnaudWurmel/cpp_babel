#include "Record.hpp"

Record::Record() {
	this->stream = NULL;
  if ((this->err = Pa_Initialize()) != paNoError)
    std::cout << "Error : " << Pa_GetErrorText(err) << '\n';
  this->state = PA_OFF;
  this->inputParameters.device = Pa_GetDefaultInputDevice();
  if (this->inputParameters.device == paNoDevice)
    return ;
  this->inputParameters.channelCount = 2;
  this->inputParameters.sampleFormat = paFloat32;
  this->inputParameters.suggestedLatency =
      Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  this->inputParameters.hostApiSpecificStreamInfo = NULL;
  this->inputSamples = new float[SAMPLE_RATE * NUM_CHANNELS];
}

Record::~Record() {
  if (this->stream && Pa_IsStreamActive(this->stream) == 1)
    Pa_CloseStream(this->stream);
}

int Record::callBackFunction(const void *&inputBuffer, void *&outputBuffer,
                             unsigned long &framesPerBuffer,
                             const PaStreamCallbackTimeInfo *&timeInfo,
                             PaStreamCallbackFlags &statusFlags) {
  // const SAMPLE *rptr = (const SAMPLE *)inputBuffer;
  // SAMPLE *wptr = this->inputSamples;

  DecodedFrame frames;
  frames.size = FRAMES_PER_BUFFER * NUM_CHANNELS;
  frames.frame.assign(reinterpret_cast<const float *>(inputBuffer),
                      reinterpret_cast<const float *>(inputBuffer) +
                          framesPerBuffer * NUM_CHANNELS);
  this->Fbuffer.push_back(frames);
  if (this->state == PA_ON)
    return paContinue;
  else
    return paComplete;
}

static int Pa_callBack(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags, void *userData) {
  Record *rec = static_cast<Record *>(userData);
  return rec->callBackFunction(inputBuffer, outputBuffer, framesPerBuffer,
                               timeInfo, statusFlags);
}

DecodedFrame Record::RecordedFrames() {
  DecodedFrame f;

  f.size = 0;
  if (this->Fbuffer.size()) {
    f = this->Fbuffer.front();
    this->Fbuffer.pop_front();
  }
  return (f);
}

bool Record::startAudio() {
	if (Pa_OpenStream(&this->stream, &this->inputParameters, NULL, SAMPLE_RATE,
		FRAMES_PER_BUFFER, paClipOff, Pa_callBack, this) == paNoError && Pa_StartStream(this->stream) == paNoError) {
		this->state = PA_ON;
		return (true);
	}
	this->stream = NULL;
	return (false);
}
bool Record::stopAudio() {
  this->state = PA_OFF;
  Pa_CloseStream(this->stream);
  return (true);
}

bool  Record::isActive() const {
  return stream && Pa_IsStreamActive(this->stream) == 1;
}
