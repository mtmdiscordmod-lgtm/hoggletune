#pragma once

#include <vector>

// ============================================================================
// DelayProcessor — Simple delay (echo) effect
// ============================================================================
// Creates echoes by storing audio and playing it back after a delay.
// The feedback parameter controls how many times the echo repeats.
//
// Parameters:
// - Delay time (ms): How long before the echo plays (20-2000ms)
// - Feedback: How much of the echo feeds back in (0-90%)
// - Wet mix: How loud the echoes are relative to the dry signal
//   Default is < 0.06 (under 6%)
// ============================================================================

class DelayProcessor
{
public:
    DelayProcessor() = default;
    ~DelayProcessor() = default;

    // Call once before processing starts
    void prepare (double sampleRate, int maxBlockSize);

    // Process audio in-place
    void process (float* samples, int numSamples);

    // Reset delay buffer (clears all echoes)
    void reset();

    // Set delay time in milliseconds (20-2000ms)
    void setDelayTimeMs (float delayMs);

    // Set feedback amount: 0.0 = single echo, 0.9 = many repeating echoes
    void setFeedback (float feedback);

    // Set wet/dry mix: 0.0 = dry only, 1.0 = wet only
    // Default is < 0.06 (under 6%)
    void setWetMix (float wet);

private:
    // Circular buffer for the delay line
    std::vector<float> delayBuffer;
    int delayBufferSize = 0;
    int writePos = 0;

    // Parameters
    float delayTimeMs = 300.0f;    // Default: 300ms delay
    float feedback = 0.3f;         // Default: 30% feedback
    float wetMix = 0.05f;          // Default: 5% wet (under 6%)

    // Derived values
    int delaySamples = 0;          // Delay time converted to samples
    double currentSampleRate = 44100.0;
};
