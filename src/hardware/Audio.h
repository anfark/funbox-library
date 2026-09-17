#pragma once

#include <stdint.h>
#include <AudioTools.h>

struct AudioConfig {
  uint8_t bclkPin;
  uint8_t lrcPin;
  uint8_t doutPin;

  int16_t volume = 3000;
};

class Audio {
public:
  explicit Audio(const AudioConfig& config);

  void setup();
  void update();

  void tone(float frequency);
  void stop();

private:
  AudioConfig _config;

  AudioInfo _info;

  I2SStream _output;

  SineGenerator<int16_t> _sine;
  GeneratedSoundStream<int16_t> _source;
  StreamCopy _copier;

  bool _playing = false;
};