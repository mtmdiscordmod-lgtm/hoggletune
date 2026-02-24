#pragma once

#include <juce_core/juce_core.h>
#include <vector>

// ============================================================================
// PitchDetector — Detects the fundamental frequency of audio using YIN
// ============================================================================
// The YIN algorithm is a well-known method for detecting pitch in audio.
// It works by comparing the audio signal to shifted versions of itself
// to find the repeating pattern (the "period") of the sound.
//
// How it works (simplified):
// 1. Take a chunk of audio samples
// 2. Compare the chunk to shifted copies of itself (the "difference function")
// 3. Normalize the comparison results
// 4. Find the shift amount that gives the best match
// 5. Convert that shift amount to a frequency (Hz)
//
// Usage:
//   PitchDetector detector;
//   detector.prepare(sampleRate);
//   detector.feedSamples(audioBuffer, numSamples);
//   float freq = detector.getDetectedFrequency();  // 0 = no pitch detected
// ============================================================================

class PitchDetector
{
public:
    PitchDetector();
    ~PitchDetector() = default;

    // Call once before processing starts. Sets the sample rate and allocates buffers.
    void prepare (double sampleRate);

    // Feed audio samples into the detector. Call this every audio buffer.
    // The detector accumulates samples internally until it has enough to analyze.
    void feedSamples (const float* samples, int numSamples);

    // Returns the last detected frequency in Hz.
    // Returns 0.0f if no pitch was detected (silence, noise, etc.)
    float getDetectedFrequency() const;

    // Returns the confidence of the detection (0.0 = no confidence, 1.0 = very confident)
    float getConfidence() const;

private:
    // Internal YIN algorithm steps
    void runYinDetection();
    float parabolicInterpolation (int tauEstimate) const;

    // Configuration
    double currentSampleRate = 44100.0;

    // The YIN buffer size — how many samples we analyze at once.
    // Larger = more accurate for low frequencies, but more latency.
    // 2048 is a good balance for voice (can detect down to ~65 Hz at 44.1kHz).
    static constexpr int kYinBufferSize = 2048;

    // YIN threshold — lower = stricter pitch detection, fewer false positives.
    // 0.15 is a typical value that works well for voice.
    static constexpr float kYinThreshold = 0.15f;

    // Internal buffers (pre-allocated, no runtime allocation)
    std::vector<float> inputBuffer;       // Accumulates incoming samples
    std::vector<float> yinBuffer;         // Stores YIN difference function results
    int inputBufferWritePos = 0;          // Where to write next in inputBuffer

    // Detection results
    float detectedFrequency = 0.0f;       // Last detected pitch in Hz
    float detectedConfidence = 0.0f;      // How confident we are (0-1)
};
