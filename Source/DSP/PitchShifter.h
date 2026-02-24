#pragma once

#include <vector>

// ============================================================================
// PitchShifter — Shifts audio pitch up or down in real-time
// ============================================================================
// This uses a circular buffer with two read heads to change the pitch of audio
// without changing its speed (unlike simply playing audio faster/slower).
//
// How it works (simplified):
// 1. Audio samples are written into a circular buffer at normal speed
// 2. Two "read heads" read from the buffer at a different speed
//    - Faster reading = higher pitch
//    - Slower reading = lower pitch
// 3. The two read heads are offset by half the buffer, and we crossfade
//    between them to avoid clicks/pops at buffer wrap points
//
// This is used both by the AutotuneEngine (small shifts to correct pitch)
// and the HarmonizerEngine (larger shifts for harmony voices).
// ============================================================================

class PitchShifter
{
public:
    PitchShifter();
    ~PitchShifter() = default;

    // Call once before processing. Allocates internal buffers.
    void prepare (double sampleRate, int maxBlockSize);

    // Set the pitch shift amount in semitones.
    // Positive = shift up, negative = shift down.
    // Range: -24 to +24 semitones (2 octaves up or down)
    void setSemitoneShift (float semitones);

    // Process a block of audio samples in-place.
    // The input samples are replaced with the pitch-shifted output.
    void process (float* samples, int numSamples);

    // Reset the internal state (call when playback stops/starts)
    void reset();

private:
    // The circular buffer that stores recent audio
    std::vector<float> circularBuffer;
    int bufferSize = 0;

    // Write position — where new samples go into the circular buffer
    int writePos = 0;

    // Two read positions (floating-point for sub-sample accuracy)
    // They're offset by half the buffer and we crossfade between them
    double readPos1 = 0.0;
    double readPos2 = 0.0;

    // The pitch ratio — how fast the read heads move relative to write head
    // 1.0 = same pitch, 2.0 = one octave up, 0.5 = one octave down
    double pitchRatio = 1.0;

    double currentSampleRate = 44100.0;

    // Size of the crossfade window (in samples)
    // Larger = smoother transitions but more "smearing"
    int crossfadeSize = 256;

    // Helper: get a sample from the circular buffer with linear interpolation
    // (needed because read positions are fractional, not integer)
    float getInterpolatedSample (double position) const;
};
