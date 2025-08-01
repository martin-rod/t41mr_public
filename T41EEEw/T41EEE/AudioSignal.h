#pragma once

#include "SDT.h"

// Uncomment for the original T41 audio mute control.
#if 1
constexpr uint8_t UNMUTEAUDIO = LOW;
constexpr uint8_t MUTEAUDIO = HIGH;
#else
// Use this for external amp with mute LOW, unmute HIGH.
constexpr uint8_t UNMUTEAUDIO = HIGH;
constexpr uint8_t MUTEAUDIO = LOW;
#endif

enum class AudioState : int { SPEAKER, HEADPHONE, BOTH, MUTE_BOTH };

extern AudioEffectGain_F32 speakerScale;
extern AudioEffectGain_F32 headphoneScale;

void initializeAudioPaths();
void SetAudioOperatingState(RadioState operatingState);
void controlAudioOut(AudioState audioState, bool mute);
