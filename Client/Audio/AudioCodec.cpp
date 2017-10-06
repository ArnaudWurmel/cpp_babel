#include "AudioCodec.hpp"

OpusCodec::OpusCodec() {
  this->FrameSize = SAMPLE_RATE;
  this->num_channels = NUM_CHANNELS;
  this->enc = opus_encoder_create(this->FrameSize, this->num_channels,
                                  OPUS_APPLICATION_VOIP, &this->error);
  this->dec =
      opus_decoder_create(this->FrameSize, this->num_channels, &this->error);
  opus_int32 size;
  opus_encoder_ctl(enc, OPUS_GET_BANDWIDTH(&size));
  this->data_size = size;
  this->ret = 0;
}
OpusCodec::~OpusCodec() {
  opus_decoder_destroy(this->dec);
  opus_encoder_destroy(this->enc);
}

DecodedFrame OpusCodec::AudioDecode(EncodedFrame frame) {
  DecodedFrame NFrame;

  NFrame.frame.resize(FRAMES_PER_BUFFER * NUM_CHANNELS);
  NFrame.size = opus_decode_float(this->dec, frame.frame.data(), frame.size,
                                  NFrame.frame.data(), FRAMES_PER_BUFFER, 0) *
                NUM_CHANNELS;
  std::cout << "decode size : " << NFrame.size << '\n';
  return (NFrame);
}

EncodedFrame OpusCodec::AudioEncode(DecodedFrame frame) {
  EncodedFrame NFrame;

  NFrame.frame.resize(frame.size);
  NFrame.size =
      opus_encode_float(this->enc, frame.frame.data(), FRAMES_PER_BUFFER,
                        NFrame.frame.data(), frame.size);
  std::cout << "Encoded new frame size : " << NFrame.size << '\n';
  return (NFrame);
}
