#ifndef IAUDIO_HPP_
#define IAUDIO_HPP_

class IAudio {
public:
  virtual bool startAudio() = 0;
  virtual bool stopAudio() = 0;
};

#endif /* IAUDIO_HPP_ */
