#include "AutotuneEngine.h"
#include <cmath>

// ============================================================================
// AutotuneEngine Implementation
// ============================================================================

AutotuneEngine::AutotuneEngine()
{
}

void AutotuneEngine::prepare (double sampleRate, int maxBlockSize)
{
    pitchDetector.prepare (sampleRate);
    pitchShifter.prepare (sampleRate, maxBlockSize);

    // Calculate smoothing coefficient based on sample rate.
    // This controls how quickly the pitch correction responds.
    // A higher coefficient = slower response = smoother correction.
    // We'll modulate this based on the correctionSpeed parameter.
    smoothingCoeff = std::exp (-1.0f / (float) (sampleRate * 0.01f));  // ~10ms base

    smoothedShift = 0.0f;
    currentDetectedPitch = 0.0f;
    currentTargetPitch = 0.0f;
}

void AutotuneEngine::process (float* samples, int numSamples)
{
    // If correction is turned off, pass through unchanged
    if (correctionSpeed <= 0.001f)
        return;

    // Step 1: Feed samples to the pitch detector
    pitchDetector.feedSamples (samples, numSamples);

    // Step 2: Get the detected pitch
    currentDetectedPitch = pitchDetector.getDetectedFrequency();

    // If no pitch is detected (silence, noise), don't try to correct
    if (currentDetectedPitch <= 0.0f)
    {
        // Gradually fade the shift back to zero when no pitch is detected
        smoothedShift *= 0.95f;
        pitchShifter.setSemitoneShift (smoothedShift);
        pitchShifter.process (samples, numSamples);
        return;
    }

    // Step 3: Find the nearest "correct" pitch in the current scale
    float targetSemitoneShift = scaleEngine.getSemitonesToNearest (currentDetectedPitch);
    currentTargetPitch = scaleEngine.getNearestScaleFrequency (currentDetectedPitch);

    // Step 4: Apply correction speed
    // correctionSpeed controls how much of the shift we actually apply:
    // - At 0%, we apply 0% of the shift (no correction)
    // - At 100%, we apply 100% of the shift (instant snap)
    float desiredShift = targetSemitoneShift * correctionSpeed;

    // Step 5: Smooth the shift to avoid sudden jumps
    // The smoothing coefficient is adjusted by correction speed:
    // - High speed = less smoothing = faster response
    // - Low speed = more smoothing = gentler correction
    float speedAdjustedSmoothing = std::pow (smoothingCoeff, 1.0f / (correctionSpeed + 0.01f));
    smoothedShift = speedAdjustedSmoothing * smoothedShift +
                    (1.0f - speedAdjustedSmoothing) * desiredShift;

    // Step 6: Apply the pitch shift
    pitchShifter.setSemitoneShift (smoothedShift);
    pitchShifter.process (samples, numSamples);
}

void AutotuneEngine::reset()
{
    pitchShifter.reset();
    smoothedShift = 0.0f;
    currentDetectedPitch = 0.0f;
    currentTargetPitch = 0.0f;
}

void AutotuneEngine::setCorrectionSpeed (float speed)
{
    // Clamp to valid range
    correctionSpeed = std::max (0.0f, std::min (1.0f, speed));
}

void AutotuneEngine::setRootNote (ScaleEngine::RootNote root)
{
    scaleEngine.setRootNote (root);
}

void AutotuneEngine::setMode (ScaleEngine::Mode mode)
{
    scaleEngine.setMode (mode);
}

float AutotuneEngine::getDetectedPitch() const
{
    return currentDetectedPitch;
}

float AutotuneEngine::getTargetPitch() const
{
    return currentTargetPitch;
}

juce::String AutotuneEngine::getDetectedNoteName() const
{
    return ScaleEngine::getNoteName (currentDetectedPitch);
}
