#include "Audio.h"


Audio::Audio(const AudioConfig& config)
  : _config(config),
    _volume(config.volume) {

  if (_volume > MAX_VOLUME) {
    _volume = MAX_VOLUME;
  }
}


void Audio::setup() {
  _info = new AudioInfo(
    44100,
    2,
    16
  );

  _output =
    new I2SStream();

  _sine =
    new SineGenerator<int16_t>(
      amplitude()
    );

  _source =
    new GeneratedSoundStream<int16_t>(
      *_sine
    );

  _copier =
    new StreamCopy(
      *_output,
      *_source
    );


  auto config =
    _output->defaultConfig(TX_MODE);

  config.copyFrom(*_info);

  config.pin_bck =
    _config.bclkPin;

  config.pin_ws =
    _config.lrcPin;

  config.pin_data =
    _config.doutPin;

  _output->begin(config);

  _sine->begin(
    *_info,
    440.0f
  );
}


void Audio::update() {
  if (!_playing) {
    return;
  }

  if (_volume == 0) {
    return;
  }

  if (_copier == nullptr) {
    return;
  }

  _copier->copy();
}


void Audio::tone(float frequency) {
  if (_sine == nullptr) {
    return;
  }

  _sine->setFrequency(
    frequency
  );

  _playing = true;
}


void Audio::stop() {
  if (_sine != nullptr) {
    _sine->setFrequency(0);
  }

  _playing = false;
}


void Audio::volume(uint8_t value) {
  if (value > MAX_VOLUME) {
    value = MAX_VOLUME;
  }

  _volume = value;

  if (_sine != nullptr) {
    _sine->setAmplitude(
      amplitude()
    );
  }
}


uint8_t Audio::volume() const {
  return _volume;
}


int16_t Audio::amplitude() const {
  return static_cast<int16_t>(
    (
      static_cast<int32_t>(
        MAX_AMPLITUDE
      )
      * _volume
    )
    / MAX_VOLUME
  );
}