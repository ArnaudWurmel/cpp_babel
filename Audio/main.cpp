#include "AudioCodec.hpp"
#include "Play.hpp"
#include "Record.hpp"
#include <iostream>

int main() {
  OpusCodec *codec = new OpusCodec;
  Play *play = new Play;
  Record *rec = new Record;
  play->startPlay();
  rec->startRecord();
  std::list<EncodedFrame> list;
  while (Pa_IsStreamActive(rec->stream) == 1) {
    list.push_back(codec->AudioEncode(rec->RecordedFrames()));
    if (list.front().size > 0)
      play->PlayFrames(codec->AudioDecode(list.front()));
    list.pop_front();
    Pa_Sleep(5);
  }
}
