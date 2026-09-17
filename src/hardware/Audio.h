#pragma once

#include <stdint.h>

#include <AudioTools.h>


struct AudioConfig {
  uint8_t bclkPin;
  uint8_t lrcPin;
  uint8_t doutPin;

  uint8_t volume;
};


class Audio {
public:
  explicit Audio(const AudioConfig& config);

  void setup();
  void update();

  void tone(float frequency);
  void stop();

  void volume(uint8_t value);
  uint8_t volume() const;

private:
  static constexpr uint8_t MAX_VOLUME = 8;
  static constexpr int16_t MAX_AMPLITUDE = 12000;

  AudioConfig _config;

  uint8_t _volume;
  bool _playing = false;

  AudioInfo* _info = nullptr;
  I2SStream* _output = nullptr;

  SineGenerator<int16_t>* _sine = nullptr;
  GeneratedSoundStream<int16_t>* _source = nullptr;
  StreamCopy* _copier = nullptr;

  int16_t amplitude() const;
};