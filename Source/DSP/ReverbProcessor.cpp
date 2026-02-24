#include "ReverbProcessor.h"
#include <cstring>
#include <algorithm>

// ============================================================================
// ReverbProcessor Implementation
// ============================================================================

void ReverbProcessor::prepare (double sampleRate, int maxBlockSize)
{
    // Set up the JUCE reverb with our sample rate
    reverb.setSampleRate (sampleRate);

    // Configure default reverb parameters
    reverbParams.roomSize = 0.5f;    // Medium room
    reverbParams.damping = 0.5f;     // Moderate high-frequency absorption
    reverbParams.wetLevel = 1.0f;    // We handle wet/dry mixing ourselves
    reverbParams.dryLevel = 0.0f;    // So reverb outputs only wet signal
    reverbParams.width = 1.0f;       // Full stereo width
    reverbParams.freezeMode = 0.0f;  // Normal mode (not infinite reverb)
    reverb.setParameters (reverbParams);

    // Pre-allocate the dry buffer for wet/dry mixing
    dryBuffer.resize ((size_t) maxBlockSize, 0.0f);
}

void ReverbProcessor::process (float* samples, int numSamples)
{
    // If reverb is essentially off, skip processing
    if (wetAmount < 0.001f)
        return;

    // Save the dry signal (we need it for wet/dry mixing)
    std::memcpy (dryBuffer.data(), samples, sizeof (float) * (size_t) numSamples);

    // Process through the JUCE reverb (mono mode)
    reverb.processMono (samples, numSamples);

    // Mix wet and dry signals
    // output = dry * (1 - wetAmount) + wet * wetAmount
    float dryLevel = 1.0f - wetAmount;

    for (int i = 0; i < numSamples; ++i)
    {
        samples[i] = dryBuffer[(size_t) i] * dryLevel + samples[i] * wetAmount;
    }
}

void ReverbProcessor::reset()
{
    reverb.reset();
}

void ReverbProcessor::setAmount (float amount)
{
    wetAmount = std::max (0.0f, std::min (1.0f, amount));
}

void ReverbProcessor::setRoomSize (float size)
{
    reverbParams.roomSize = std::max (0.0f, std::min (1.0f, size));
    reverb.setParameters (reverbParams);
}
