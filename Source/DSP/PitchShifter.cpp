#include "PitchShifter.h"
#include <cmath>
#include <algorithm>

// ============================================================================
// PitchShifter Implementation — Circular Buffer with Dual Read Heads
// ============================================================================

PitchShifter::PitchShifter()
{
}

void PitchShifter::prepare (double sampleRate, int /*maxBlockSize*/)
{
    currentSampleRate = sampleRate;

    // Buffer size: ~100ms worth of audio. This determines the maximum
    // "window" we can work with for pitch shifting.
    bufferSize = (int) (sampleRate * 0.1);

    // Make buffer size a power of 2 for efficient wrapping
    int powerOf2 = 1;
    while (powerOf2 < bufferSize)
        powerOf2 *= 2;
    bufferSize = powerOf2;

    circularBuffer.resize ((size_t) bufferSize, 0.0f);

    // Crossfade size: ~5ms worth of samples
    crossfadeSize = (int) (sampleRate * 0.005);
    crossfadeSize = std::max (crossfadeSize, 64);

    reset();
}

void PitchShifter::setSemitoneShift (float semitones)
{
    // Convert semitones to a pitch ratio.
    // The formula: ratio = 2^(semitones/12)
    // Examples:
    //   0 semitones  → ratio = 1.0 (no change)
    //   12 semitones → ratio = 2.0 (one octave up)
    //  -12 semitones → ratio = 0.5 (one octave down)
    //   7 semitones  → ratio ≈ 1.498 (perfect fifth up)
    pitchRatio = std::pow (2.0, (double) semitones / 12.0);
}

void PitchShifter::reset()
{
    // Clear the buffer and reset positions
    std::fill (circularBuffer.begin(), circularBuffer.end(), 0.0f);
    writePos = 0;

    // Start read heads at offset positions
    readPos1 = 0.0;
    readPos2 = (double) bufferSize / 2.0;
}

void PitchShifter::process (float* samples, int numSamples)
{
    // If pitch ratio is exactly 1.0, no shifting needed — pass through
    if (std::abs (pitchRatio - 1.0) < 0.0001)
        return;

    int bufferMask = bufferSize - 1;  // For fast modulo (works because bufferSize is power of 2)

    for (int i = 0; i < numSamples; ++i)
    {
        // Write the input sample into the circular buffer
        circularBuffer[(size_t) (writePos & bufferMask)] = samples[i];

        // Read from both read heads (with interpolation for smooth output)
        float sample1 = getInterpolatedSample (readPos1);
        float sample2 = getInterpolatedSample (readPos2);

        // Calculate crossfade amounts based on where each read head is
        // relative to the write position. We fade out a read head as it
        // approaches the write position (to avoid reading samples that
        // haven't been written yet).

        // Distance from each read head to the write head
        double dist1 = std::fmod ((double) writePos - readPos1 + (double) bufferSize,
                                  (double) bufferSize);
        double dist2 = std::fmod ((double) writePos - readPos2 + (double) bufferSize,
                                  (double) bufferSize);

        // Crossfade weights: fade in/out smoothly near the edges
        float fade1 = 1.0f;
        float fade2 = 1.0f;

        double halfBuffer = (double) bufferSize / 2.0;

        // Fade based on proximity to write position
        if (dist1 < (double) crossfadeSize)
            fade1 = (float) (dist1 / (double) crossfadeSize);
        else if (dist1 > halfBuffer * 2.0 - (double) crossfadeSize)
            fade1 = (float) ((halfBuffer * 2.0 - dist1) / (double) crossfadeSize);

        if (dist2 < (double) crossfadeSize)
            fade2 = (float) (dist2 / (double) crossfadeSize);
        else if (dist2 > halfBuffer * 2.0 - (double) crossfadeSize)
            fade2 = (float) ((halfBuffer * 2.0 - dist2) / (double) crossfadeSize);

        // Clamp fades to valid range
        fade1 = std::max (0.0f, std::min (1.0f, fade1));
        fade2 = std::max (0.0f, std::min (1.0f, fade2));

        // Mix the two read heads
        float totalFade = fade1 + fade2;
        float output = 0.0f;
        if (totalFade > 0.0f)
            output = (sample1 * fade1 + sample2 * fade2) / totalFade;

        samples[i] = output;

        // Advance the write position by 1 (normal speed)
        writePos = (writePos + 1) & bufferMask;

        // Advance the read positions by the pitch ratio
        // pitchRatio > 1.0 = reading faster = higher pitch
        // pitchRatio < 1.0 = reading slower = lower pitch
        readPos1 += pitchRatio;
        readPos2 += pitchRatio;

        // Wrap read positions around the buffer
        if (readPos1 >= (double) bufferSize) readPos1 -= (double) bufferSize;
        if (readPos2 >= (double) bufferSize) readPos2 -= (double) bufferSize;
        if (readPos1 < 0.0) readPos1 += (double) bufferSize;
        if (readPos2 < 0.0) readPos2 += (double) bufferSize;
    }
}

float PitchShifter::getInterpolatedSample (double position) const
{
    // Linear interpolation between two adjacent samples.
    // Since our read position is a fractional number (e.g., 42.7),
    // we blend between sample[42] and sample[43] based on the fraction (0.7).

    int bufferMask = bufferSize - 1;

    int index0 = (int) position & bufferMask;
    int index1 = (index0 + 1) & bufferMask;
    float fraction = (float) (position - std::floor (position));

    float s0 = circularBuffer[(size_t) index0];
    float s1 = circularBuffer[(size_t) index1];

    return s0 + fraction * (s1 - s0);
}
