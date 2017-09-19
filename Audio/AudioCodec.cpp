#include "AudioCodec.hpp"

AudioCodec::AudioCodec() {
  this->FrameSize = 24000;
  this->num_channels = 2;
  this->enc = opus_encoder_create(this->FrameSize, this->num_channels,
                                  OPUS_APPLICATION_VOIP, &this->error);
  this->dec =
      opus_decoder_create(this->FrameSize, this->num_channels, &this->error);
  opus_int32 size;
  opus_encoder_ctl(enc, OPUS_GET_BANDWIDTH(&size));
  this->data_size = size;
}
AudioCodec::~AudioCodec() {}

float *AudioCodec::AudioDecode(unsigned char *data) {
  float *frame = new float[this->FrameSize * this->num_channels];

  memset(frame, '\0', sizeof(float) * (this->FrameSize * this->num_channels));
  int ret = opus_decode_float(this->dec, data, this->ret, frame, 480, 0);
  return (frame);
}

unsigned char *AudioCodec::AudioEncode(float *frame) {
  unsigned char *buff = new unsigned char[this->data_size];

  bzero(buff, this->data_size);
  this->ret = opus_encode_float(this->enc, frame, 480, buff, this->data_size);
  std::cout << "Encode len : " << ret << std::endl;
  return (buff);
}
