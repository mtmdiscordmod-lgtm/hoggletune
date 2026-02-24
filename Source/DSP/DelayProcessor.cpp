#include "DelayProcessor.h"
#include <algorithm>
#include <cmath>

// ============================================================================
// DelayProcessor Implementation
// ============================================================================

void DelayProcessor::prepare (double sampleRate, int /*maxBlockSize*/)
{
    currentSampleRate = sampleRate;

    // Allocate enough buffer for 2 seconds of delay (the maximum)
    delayBufferSize = (int) (sampleRate * 2.0) + 1;
    delayBuffer.resize ((size_t) delayBufferSize, 0.0f);

    // Calculate delay in samples from the current delay time
    delaySamples = (int) (delayTimeMs * sampleRate / 1000.0);
    delaySamples = std::max (1, std::min (delaySamples, delayBufferSize - 1));

    writePos = 0;
}

void DelayProcessor::process (float* samples, int numSamples)
{
    // If delay is essentially off, skip processing
    if (wetMix < 0.001f)
        return;

    for (int i = 0; i < numSamples; ++i)
    {
        // Calculate the read position (where to read the delayed sample from)
        int readPos = writePos - delaySamples;
        if (readPos < 0)
            readPos += delayBufferSize;

        // Read the delayed sample
        float delayedSample = delayBuffer[(size_t) readPos];

        // Write the current input + feedback into the delay buffer
        // Feedback causes the echo to repeat — higher feedback = more repeats
        delayBuffer[(size_t) writePos] = samples[i] + delayedSample * feedback;

        // Mix the delayed signal with the dry signal
        // output = dry * (1 - wet) + delayed * wet
        samples[i] = samples[i] * (1.0f - wetMix) + delayedSample * wetMix;

        // Advance the write position
        writePos++;
        if (writePos >= delayBufferSize)
            writePos = 0;
    }
}

void DelayProcessor::reset()
{
    std::fill (delayBuffer.begin(), delayBuffer.end(), 0.0f);
    writePos = 0;
}

void DelayProcessor::setDelayTimeMs (float delayMs)
{
    delayTimeMs = std::max (20.0f, std::min (2000.0f, delayMs));
    delaySamples = (int) (delayTimeMs * currentSampleRate / 1000.0);
    delaySamples = std::max (1, std::min (delaySamples, delayBufferSize - 1));
}

void DelayProcessor::setFeedback (float fb)
{
    // Cap feedback at 0.9 to prevent infinite buildup
    feedback = std::max (0.0f, std::min (0.9f, fb));
}

void DelayProcessor::setWetMix (float wet)
{
    wetMix = std::max (0.0f, std::min (1.0f, wet));
}
