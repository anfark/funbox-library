#include "Audio.h"

Audio::Audio(const AudioConfig& config)
  : _config(config),
    _info(44100, 2, 16),
    _sine(config.volume),
    _source(_sine),
    _copier(_output, _source) {
}

void Audio::setup() {
  auto config = _output.defaultConfig(TX_MODE);

  config.copyFrom(_info);

  config.pin_bck = _config.bclkPin;
  config.pin_ws = _config.lrcPin;
  config.pin_data = _config.doutPin;

  _output.begin(config);

  _sine.begin(
    _info,
    440.0f
  );
}

void Audio::update() {
  if (!_playing) {
    return;
  }

  _copier.copy();
}

void Audio::tone(float frequency) {
  _sine.setFrequency(frequency);
  _playing = true;
}

void Audio::stop() {
  _sine.setFrequency(0);
  _playing = false;
}