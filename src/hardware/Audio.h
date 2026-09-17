#pragma once

#include <stdint.h>
#include <AudioTools.h>

struct AudioConfig {
  uint8_t bclkPin;
  uint8_t lrcPin;
  uint8_t doutPin;
  int16_t volume = 3000;
};

class AudioChannel {
public:
  AudioChannel(
    OutputMixer<int16_t>& mixer,
    uint8_t mixerIndex,
    const AudioInfo& info,
    int16_t volume
  );

  void setup();

  void tone(float frequency);
  void stop();

  void update();

private:
  const AudioInfo& _info;

  SineGenerator<int16_t> _sine;
  GeneratedSoundStream<int16_t> _source;
  StreamCopy _copier;

  bool _playing = false;
};


class Audio {
public:
  explicit Audio(const AudioConfig& config);

  void setup();
  void update();

  AudioChannel& music();
  AudioChannel& sfx();

private:
  AudioConfig _config;

  AudioInfo _info;

  I2SStream _output;

  OutputMixer<int16_t> _mixer;

  AudioChannel _music;
  AudioChannel _sfx;
};