#pragma once

#include <juce_dsp/juce_dsp.h>

// ============================================================================
// ReverbProcessor — Simple reverb effect
// ============================================================================
// Adds a sense of space/room to the audio using JUCE's built-in Reverb class.
// The Reverb class implements a Schroeder reverb — a classic algorithm that
// simulates sound bouncing around a room.
//
// The "amount" parameter controls the wet/dry mix:
// - 0% = completely dry (no reverb)
// - 10% = subtle room feel (the default for HoggleTune)
// - 50% = prominent reverb
// - 100% = fully wet (only reverb, no dry signal)
// ============================================================================

class ReverbProcessor
{
public:
    ReverbProcessor() = default;
    ~ReverbProcessor() = default;

    // Call once before processing starts
    void prepare (double sampleRate, int maxBlockSize);

    // Process audio in-place
    void process (float* samples, int numSamples);

    // Reset reverb state (clears the reverb tail)
    void reset();

    // Set reverb amount (wet/dry mix): 0.0 = dry, 1.0 = fully wet
    // Default is < 0.1 (under 10%)
    void setAmount (float amount);

    // Set room size: 0.0 = tiny room, 1.0 = huge hall
    void setRoomSize (float size);

private:
    juce::Reverb reverb;
    juce::Reverb::Parameters reverbParams;

    float wetAmount = 0.08f;  // Default: 8% (under 10%)

    // Temp buffer for dry signal (for wet/dry mixing)
    std::vector<float> dryBuffer;
};
