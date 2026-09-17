#include "Audio.h"


AudioChannel::AudioChannel(
  OutputMixer<int16_t>& mixer,
  uint8_t mixerIndex,
  const AudioInfo& info,
  int16_t volume
)
  : _info(info),
    _sine(volume),
    _source(_sine),
    _copier() {

  _copier.begin(
    mixer[mixerIndex],
    _source
  );
}


void AudioChannel::setup() {
  _source.begin(_info);

  _sine.begin(
    _info,
    440.0f
  );
}


void AudioChannel::tone(float frequency) {
  _sine.setFrequency(frequency);
  _playing = true;
}


void AudioChannel::stop() {
  _playing = false;
}


void AudioChannel::update() {
  if (!_playing) {
    return;
  }

  _copier.copy();
}


// --------------------------------------------------


Audio::Audio(const AudioConfig& config)
  : _config(config),
    _info(44100, 2, 16),
    _mixer(_output, 2, DefaultAllocatorRAM),
    _music(
      _mixer,
      0,
      _info,
      config.volume
    ),
    _sfx(
      _mixer,
      1,
      _info,
      config.volume
    ) {}


void Audio::setup() {
  auto config =
    _output.defaultConfig(TX_MODE);

  config.copyFrom(_info);

  config.pin_bck = _config.bclkPin;
  config.pin_ws = _config.lrcPin;
  config.pin_data = _config.doutPin;

  _output.begin(config);

  _mixer.begin();

  _music.setup();
  _sfx.setup();
}


void Audio::update() {
  _music.update();
  _sfx.update();
}


AudioChannel& Audio::music() {
  return _music;
}


AudioChannel& Audio::sfx() {
  return _sfx;
}