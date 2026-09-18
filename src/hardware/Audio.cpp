#include "Audio.h"

#include <Arduino.h>
#include <type_traits>
#include <variant>


Audio::Audio(
  const AudioConfig& config
)
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
    new SineWaveGenerator<int16_t>(
      amplitude()
    );

  _noise =
    new WhiteNoiseGenerator<int16_t>(
      noiseAmplitude()
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
    0.0f
  );

  _noise->begin(
    *_info
  );

  useSilence();
}


void Audio::update() {
  const uint32_t now =
    millis();

  updateMusic(now);
  updateEffect(now);

  updateOutput();

  if (
    _volume == 0 ||
    _copier == nullptr ||
    _outputMode == OutputMode::Silence
  ) {
    return;
  }

  _copier->copy();
}


void Audio::music(
  const Melody& melody
) {
  startMusic(melody);
  updateOutput();
}


void Audio::effect(
  const SoundEffect& effect
) {
  std::visit(
    [this](const auto& value) {
      startEffect(value);
    },
    effect
  );

  updateOutput();
}


void Audio::stopMusic() {
  _music.playing = false;
  updateOutput();
}


void Audio::stopEffect() {
  _effect.playing = false;
  updateOutput();
}


void Audio::stop() {
  _music.playing = false;
  _effect.playing = false;

  updateOutput();
}


bool Audio::isMusicPlaying() const {
  return _music.playing;
}


bool Audio::isEffectPlaying() const {
  return _effect.playing;
}


void Audio::volume(
  uint8_t value
) {
  if (value > MAX_VOLUME) {
    value = MAX_VOLUME;
  }

  _volume = value;

  if (_sine != nullptr) {
    _sine->setAmplitude(
      amplitude()
    );
  }

  if (_noise != nullptr) {
    const bool wasUsingNoise =
      _outputMode == OutputMode::Noise;

    delete _noise;

    _noise =
      new WhiteNoiseGenerator<int16_t>(
        noiseAmplitude()
      );

    if (_info != nullptr) {
      _noise->begin(
        *_info
      );
    }

    if (
      wasUsingNoise &&
      _source != nullptr
    ) {
      _source->setInput(
        *_noise
      );
    }
  }
}


uint8_t Audio::volume() const {
  return _volume;
}


void Audio::startMusic(
  const Melody& melody
) {
  _music.melody = melody;
  _music.index = 0;
  _music.startedAt = millis();
  _music.loop = true;

  _music.playing =
    !_music.melody.empty();
}


void Audio::startEffect(
  const Melody& melody
) {
  _effect.effect = melody;
  _effect.melodyIndex = 0;
  _effect.startedAt = millis();
  _effect.playing = !melody.empty();
}


void Audio::startEffect(
  const Noise& noise
) {
  _effect.effect = noise;
  _effect.melodyIndex = 0;
  _effect.startedAt = millis();
  _effect.playing = true;
}


void Audio::updateMusic(
  uint32_t now
) {
  if (
    !_music.playing ||
    _music.melody.empty()
  ) {
    return;
  }

  const Note& note =
    _music.melody[
      _music.index
    ];

  if (
    now - _music.startedAt <
    durationMs(
      note.duration,
      BPM
    )
  ) {
    return;
  }

  ++_music.index;

  if (
    _music.index >=
    _music.melody.size()
  ) {
    _music.index = 0;
  }

  _music.startedAt = now;
}


void Audio::updateEffect(
  uint32_t now
) {
  if (!_effect.playing) {
    return;
  }

  std::visit(
    [this, now](const auto& value) {
      using T =
        std::decay_t<decltype(value)>;

      if constexpr (
        std::is_same_v<T, Melody>
      ) {
        if (value.empty()) {
          _effect.playing = false;
          return;
        }

        const Note& note =
          value[
            _effect.melodyIndex
          ];

        if (
          now - _effect.startedAt <
          durationMs(
            note.duration,
            BPM
          )
        ) {
          return;
        }

        ++_effect.melodyIndex;

        if (
          _effect.melodyIndex >=
          value.size()
        ) {
          _effect.playing = false;
          return;
        }

        _effect.startedAt = now;
      }

      else if constexpr (
        std::is_same_v<T, Noise>
      ) {
        if (
          now - _effect.startedAt >=
          durationMs(
            value.duration,
            BPM
          )
        ) {
          _effect.playing = false;
        }
      }
    },
    _effect.effect
  );
}


void Audio::updateOutput() {
  if (_effect.playing) {
    std::visit(
      [this](const auto& value) {
        using T =
          std::decay_t<decltype(value)>;

        if constexpr (
          std::is_same_v<T, Melody>
        ) {
          useTone(
            currentEffectFrequency()
          );
        }

        else if constexpr (
          std::is_same_v<T, Noise>
        ) {
          useNoise();
        }
      },
      _effect.effect
    );

    return;
  }

  if (_music.playing) {
    useTone(
      currentMusicFrequency()
    );

    return;
  }

  useSilence();
}


void Audio::useSilence() {
  if (
    _source == nullptr ||
    _sine == nullptr
  ) {
    return;
  }

  if (
    _outputMode ==
    OutputMode::Silence
  ) {
    return;
  }

  _source->setInput(
    *_sine
  );

  _sine->setFrequency(
    0.0f
  );

  _outputMode =
    OutputMode::Silence;

  _currentFrequency =
    0.0f;
}


void Audio::useTone(
  float frequency
) {
  if (
    _source == nullptr ||
    _sine == nullptr
  ) {
    return;
  }

  if (
    _outputMode !=
    OutputMode::Tone
  ) {
    _source->setInput(
      *_sine
    );

    _outputMode =
      OutputMode::Tone;

    _currentFrequency =
      -1.0f;
  }

  if (
    frequency ==
    _currentFrequency
  ) {
    return;
  }

  _currentFrequency =
    frequency;

  _sine->setFrequency(
    frequency
  );
}


void Audio::useNoise() {
  if (
    _source == nullptr ||
    _noise == nullptr
  ) {
    return;
  }

  if (
    _outputMode ==
    OutputMode::Noise
  ) {
    return;
  }

  _source->setInput(
    *_noise
  );

  _outputMode =
    OutputMode::Noise;

  _currentFrequency =
    -1.0f;
}


float Audio::currentMusicFrequency() const {
  if (
    !_music.playing ||
    _music.melody.empty()
  ) {
    return 0.0f;
  }

  return _music
    .melody[_music.index]
    .frequency;
}


float Audio::currentEffectFrequency() const {
  if (!_effect.playing) {
    return 0.0f;
  }

  const Melody* melody =
    std::get_if<Melody>(
      &_effect.effect
    );

  if (
    melody == nullptr ||
    melody->empty()
  ) {
    return 0.0f;
  }

  return (*melody)[
    _effect.melodyIndex
  ].frequency;
}


int16_t Audio::amplitude() const {
  return static_cast<int16_t>(
    (
      static_cast<int32_t>(
        MAX_AMPLITUDE
      ) *
      _volume
    ) /
    MAX_VOLUME
  );
}


int16_t Audio::noiseAmplitude() const {
  return static_cast<int16_t>(
    amplitude() * NOISE_VOLUME
  );
}