#pragma once

#include "PitchShifter.h"
#include <array>

// ============================================================================
// HarmonizerEngine — Creates up to 4 harmony voices
// ============================================================================
// A harmonizer takes the input audio and creates copies at different pitches
// (intervals). For example, if you sing a C and add a voice at +4 semitones,
// you'll hear both the C and an E — creating a major third harmony.
//
// Each voice has:
// - An interval (how many semitones up or down from the original)
// - A volume level (0.0 to 1.0)
// - An enable/disable switch
//
// Common intervals:
//   +3 or +4 = third (minor/major)
//   +5 = fourth
//   +7 = fifth
//   +12 = octave up
//   -12 = octave down
// ============================================================================

class HarmonizerEngine
{
public:
    // Maximum number of harmony voices
    static constexpr int kMaxVoices = 4;

    HarmonizerEngine();
    ~HarmonizerEngine() = default;

    // Call once before processing starts
    void prepare (double sampleRate, int maxBlockSize);

    // Process audio and MIX harmony voices into the output.
    // The input buffer is read (not modified). Harmony output is added to outputBuffer.
    void process (const float* inputSamples, float* outputBuffer, int numSamples);

    // Reset all internal state
    void reset();

    // ---- Per-voice parameters ----

    // Set the interval for a specific voice (in semitones, -24 to +24)
    void setVoiceInterval (int voiceIndex, float semitones);

    // Set the volume for a specific voice (0.0 to 1.0)
    void setVoiceGain (int voiceIndex, float gain);

    // Enable or disable a specific voice
    void setVoiceEnabled (int voiceIndex, bool enabled);

private:
    // Each voice is an independent PitchShifter
    struct HarmonyVoice
    {
        PitchShifter shifter;
        float interval = 0.0f;     // Semitones
        float gain = 0.5f;         // Volume (0-1)
        bool enabled = false;      // On/off
    };

    std::array<HarmonyVoice, kMaxVoices> voices;

    // Temporary buffer for processing each voice (pre-allocated)
    std::vector<float> tempBuffer;
};
