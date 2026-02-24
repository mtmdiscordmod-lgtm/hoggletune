#include "DistortionProcessor.h"
#include <cmath>
#include <algorithm>

// ============================================================================
// DistortionProcessor Implementation
// ============================================================================

void DistortionProcessor::prepare (double /*sampleRate*/)
{
    // Distortion doesn't need sample-rate-dependent setup,
    // but we keep the interface consistent with other processors.
}

void DistortionProcessor::process (float* samples, int numSamples)
{
    // If distortion is essentially off, skip processing entirely
    if (distortionAmount < 0.001f)
        return;

    for (int i = 0; i < numSamples; ++i)
    {
        samples[i] = softClip (samples[i], distortionAmount);
    }
}

void DistortionProcessor::setAmount (float amount)
{
    distortionAmount = std::max (0.0f, std::min (1.0f, amount));
}

float DistortionProcessor::softClip (float sample, float drive)
{
    // Scale the drive from 0-1 range to a useful overdrive range (1-20).
    // At drive=0, the multiplier is 1 (no change).
    // At drive=1, the multiplier is 20 (heavy overdrive).
    float driveScale = 1.0f + drive * 19.0f;

    // Overdrive the signal
    float driven = sample * driveScale;

    // Apply tanh soft clipping.
    // tanh(x) smoothly maps any value to the range (-1, +1).
    // Small values pass through almost unchanged.
    // Large values get squashed toward +-1.
    float clipped = std::tanh (driven);

    // Compensate for volume increase from the drive
    // (so turning up distortion doesn't also make it louder)
    float gainCompensation = 1.0f / std::tanh (driveScale);

    return clipped * gainCompensation;
}
