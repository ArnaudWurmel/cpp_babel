#ifndef AUDIOSETTINGS_H
#define AUDIOSETTINGS_H

#include <vector>

typedef float SAMPLE;
#define SAMPLE_RATE (24000)
#define NUM_CHANNELS (2)
#define FRAMES_PER_BUFFER (480)

enum { PA_OFF = 0, PA_ON };

struct DecodedFrame {
  std::vector<float> frame;
  int size;
};

struct EncodedFrame {
  std::vector<unsigned char> frame;
  int size;
};

#endif
