#pragma once

#include <stdint.h>

#include <AudioTools.h>

#include "core/Note.h"
#include "core/SoundEffect.h"


struct AudioConfig {
  uint8_t bclkPin;
  uint8_t lrcPin;
  uint8_t doutPin;

  uint8_t volume;
};


class Audio {
public:
  explicit Audio(
    const AudioConfig& config
  );

  void setup();
  void update();

  void music(
    const Melody& melody
  );

  void effect(
    const SoundEffect& effect
  );

  void stopMusic();
  void stopEffect();
  void stop();

  bool isMusicPlaying() const;
  bool isEffectPlaying() const;

  void volume(uint8_t value);
  uint8_t volume() const;

private:
  static constexpr uint8_t MAX_VOLUME = 8;
  static constexpr int16_t MAX_AMPLITUDE = 12000;

  static constexpr float NOISE_VOLUME = 0.5f;

  static constexpr uint16_t BPM = 120;

  struct MelodyChannel {
    Melody melody;

    size_t index = 0;
    uint32_t startedAt = 0;

    bool playing = false;
    bool loop = false;
  };

  struct EffectChannel {
    SoundEffect effect;

    size_t melodyIndex = 0;
    uint32_t startedAt = 0;

    bool playing = false;
  };

  enum class OutputMode {
    Silence,
    Tone,
    Noise
  };

  AudioConfig _config;

  uint8_t _volume;

  AudioInfo* _info = nullptr;
  I2SStream* _output = nullptr;

  SineWaveGenerator<int16_t>* _sine = nullptr;
  WhiteNoiseGenerator<int16_t>* _noise = nullptr;

  GeneratedSoundStream<int16_t>* _source = nullptr;
  StreamCopy* _copier = nullptr;

  MelodyChannel _music;
  EffectChannel _effect;

  OutputMode _outputMode =
    OutputMode::Silence;

  float _currentFrequency = -1.0f;

  void startMusic(
    const Melody& melody
  );

  void startEffect(
    const Melody& melody
  );

  void startEffect(
    const Noise& noise
  );

  void updateMusic(
    uint32_t now
  );

  void updateEffect(
    uint32_t now
  );

  void updateOutput();

  void useSilence();
  void useTone(float frequency);
  void useNoise();

  float currentMusicFrequency() const;
  float currentEffectFrequency() const;

  int16_t amplitude() const;
  int16_t noiseAmplitude() const;
};