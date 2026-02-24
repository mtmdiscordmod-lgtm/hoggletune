#pragma once

#include "PitchDetector.h"
#include "PitchShifter.h"
#include "ScaleEngine.h"

// ============================================================================
// AutotuneEngine — Automatic pitch correction
// ============================================================================
// This is the main autotune component. It:
// 1. Detects the pitch of the incoming audio (using PitchDetector)
// 2. Finds the nearest "correct" pitch in the selected scale (using ScaleEngine)
// 3. Shifts the audio to that correct pitch (using PitchShifter)
//
// The "correction speed" parameter controls how aggressively pitch is corrected:
// - 0% speed = no correction (pass-through)
// - 50% speed = gentle, natural-sounding correction
// - 100% speed = instant snap to pitch (the "T-Pain effect")
// ============================================================================

class AutotuneEngine
{
public:
    AutotuneEngine();
    ~AutotuneEngine() = default;

    // Call once before processing starts
    void prepare (double sampleRate, int maxBlockSize);

    // Process audio in-place. This detects pitch and applies correction.
    void process (float* samples, int numSamples);

    // Reset internal state
    void reset();

    // ---- Parameters ----

    // Set correction speed: 0.0 = off, 1.0 = instant snap
    void setCorrectionSpeed (float speed);

    // Set the musical key and mode for pitch correction
    void setRootNote (ScaleEngine::RootNote root);
    void setMode (ScaleEngine::Mode mode);

    // ---- Readouts (for the GUI) ----

    // Get the currently detected pitch in Hz (0 = no pitch)
    float getDetectedPitch() const;

    // Get the target pitch we're correcting toward
    float getTargetPitch() const;

    // Get the detected pitch as a note name (e.g., "A4", "C#3")
    juce::String getDetectedNoteName() const;

private:
    PitchDetector pitchDetector;
    PitchShifter pitchShifter;
    ScaleEngine scaleEngine;

    float correctionSpeed = 0.5f;  // Default: moderate correction

    // Current state (updated each processing block)
    float currentDetectedPitch = 0.0f;
    float currentTargetPitch = 0.0f;

    // Smoothed semitone shift (to avoid jumpy corrections)
    float smoothedShift = 0.0f;
    float smoothingCoeff = 0.0f;
};
