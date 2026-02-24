#pragma once

// ============================================================================
// DistortionProcessor — Soft-clip distortion effect
// ============================================================================
// Applies a gentle distortion by "soft-clipping" the audio signal.
// Unlike hard clipping (which chops off the waveform abruptly),
// soft clipping gradually rounds off the peaks, producing a warmer sound.
//
// The amount parameter controls how much the signal is overdriven:
// - 0% = no distortion (clean pass-through)
// - 10% = subtle warmth (the default for HoggleTune)
// - 50% = moderate overdrive
// - 100% = heavy distortion
// ============================================================================

class DistortionProcessor
{
public:
    DistortionProcessor() = default;
    ~DistortionProcessor() = default;

    // Call once before processing starts
    void prepare (double sampleRate);

    // Process audio in-place
    void process (float* samples, int numSamples);

    // Set distortion amount: 0.0 = clean, 1.0 = maximum distortion
    // Default is < 0.1 (under 10%)
    void setAmount (float amount);

private:
    float distortionAmount = 0.08f;  // Default: 8% (under 10%)

    // Soft-clip function using tanh (hyperbolic tangent).
    // tanh naturally curves toward +-1 as input grows, creating smooth clipping.
    static float softClip (float sample, float drive);
};
