#include "HarmonizerEngine.h"
#include <algorithm>
#include <cstring>

// ============================================================================
// HarmonizerEngine Implementation
// ============================================================================

HarmonizerEngine::HarmonizerEngine()
{
}

void HarmonizerEngine::prepare (double sampleRate, int maxBlockSize)
{
    // Prepare each voice's pitch shifter
    for (auto& voice : voices)
    {
        voice.shifter.prepare (sampleRate, maxBlockSize);
    }

    // Pre-allocate the temp buffer so we don't allocate during processing
    tempBuffer.resize ((size_t) maxBlockSize, 0.0f);
}

void HarmonizerEngine::process (const float* inputSamples, float* outputBuffer, int numSamples)
{
    // For each enabled voice:
    // 1. Copy the input into a temp buffer
    // 2. Pitch-shift the temp buffer by the voice's interval
    // 3. Add the shifted audio to the output buffer (mixed with gain)

    for (auto& voice : voices)
    {
        if (!voice.enabled || std::abs (voice.gain) < 0.001f)
            continue;

        // Copy input to temp buffer (we need a copy because PitchShifter works in-place)
        std::memcpy (tempBuffer.data(), inputSamples, sizeof (float) * (size_t) numSamples);

        // Set the pitch shift for this voice
        voice.shifter.setSemitoneShift (voice.interval);

        // Apply pitch shifting to the temp buffer
        voice.shifter.process (tempBuffer.data(), numSamples);

        // Add the shifted audio to the output, scaled by the voice's gain
        for (int i = 0; i < numSamples; ++i)
        {
            outputBuffer[i] += tempBuffer[(size_t) i] * voice.gain;
        }
    }
}

void HarmonizerEngine::reset()
{
    for (auto& voice : voices)
    {
        voice.shifter.reset();
    }
}

void HarmonizerEngine::setVoiceInterval (int voiceIndex, float semitones)
{
    if (voiceIndex >= 0 && voiceIndex < kMaxVoices)
    {
        // Clamp to -24 to +24 semitones (2 octaves)
        voices[(size_t) voiceIndex].interval = std::max (-24.0f, std::min (24.0f, semitones));
    }
}

void HarmonizerEngine::setVoiceGain (int voiceIndex, float gain)
{
    if (voiceIndex >= 0 && voiceIndex < kMaxVoices)
    {
        voices[(size_t) voiceIndex].gain = std::max (0.0f, std::min (1.0f, gain));
    }
}

void HarmonizerEngine::setVoiceEnabled (int voiceIndex, bool enabled)
{
    if (voiceIndex >= 0 && voiceIndex < kMaxVoices)
    {
        voices[(size_t) voiceIndex].enabled = enabled;
    }
}
