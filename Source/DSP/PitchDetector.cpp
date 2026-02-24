#include "PitchDetector.h"
#include <cmath>

// ============================================================================
// PitchDetector Implementation — YIN Algorithm
// ============================================================================
// Reference: "YIN, a fundamental frequency estimator for speech and music"
// by Alain de Cheveigne and Hideki Kawahara (2002)
// ============================================================================

PitchDetector::PitchDetector()
{
}

void PitchDetector::prepare (double sampleRate)
{
    currentSampleRate = sampleRate;

    // Allocate the input buffer — we need 2x the YIN buffer size
    // because YIN compares the first half against the second half.
    inputBuffer.resize (kYinBufferSize * 2, 0.0f);
    inputBufferWritePos = 0;

    // The YIN buffer stores the normalized difference function.
    // It has one entry for each possible "lag" (shift amount).
    yinBuffer.resize (kYinBufferSize, 0.0f);

    detectedFrequency = 0.0f;
    detectedConfidence = 0.0f;
}

void PitchDetector::feedSamples (const float* samples, int numSamples)
{
    // Accumulate incoming samples into our internal buffer.
    // When we have enough samples (2 * kYinBufferSize), run detection.
    for (int i = 0; i < numSamples; ++i)
    {
        inputBuffer[(size_t) inputBufferWritePos] = samples[i];
        inputBufferWritePos++;

        // Once we have enough samples, run the YIN algorithm
        if (inputBufferWritePos >= kYinBufferSize * 2)
        {
            runYinDetection();
            inputBufferWritePos = 0;
        }
    }
}

float PitchDetector::getDetectedFrequency() const
{
    return detectedFrequency;
}

float PitchDetector::getConfidence() const
{
    return detectedConfidence;
}

void PitchDetector::runYinDetection()
{
    const int halfBuffer = kYinBufferSize;

    // ---- Step 1: Difference Function ----
    // For each possible lag (tau), compute how different the signal is
    // from a shifted version of itself. A low value means the signal
    // repeats at that lag — that's the pitch period.
    yinBuffer[0] = 1.0f;  // By definition

    for (int tau = 1; tau < halfBuffer; ++tau)
    {
        float sum = 0.0f;
        for (int j = 0; j < halfBuffer; ++j)
        {
            float delta = inputBuffer[(size_t) j] - inputBuffer[(size_t) (j + tau)];
            sum += delta * delta;
        }
        yinBuffer[(size_t) tau] = sum;
    }

    // ---- Step 2: Cumulative Mean Normalized Difference ----
    // Normalize the difference function so we can use a fixed threshold.
    // This is what makes YIN better than plain autocorrelation.
    float runningSum = 0.0f;
    yinBuffer[0] = 1.0f;

    for (int tau = 1; tau < halfBuffer; ++tau)
    {
        runningSum += yinBuffer[(size_t) tau];
        if (runningSum > 0.0f)
            yinBuffer[(size_t) tau] *= (float) tau / runningSum;
        else
            yinBuffer[(size_t) tau] = 1.0f;
    }

    // ---- Step 3: Absolute Threshold ----
    // Find the first lag where the normalized difference drops below
    // our threshold. This is likely the pitch period.
    int tauEstimate = -1;

    for (int tau = 2; tau < halfBuffer; ++tau)
    {
        if (yinBuffer[(size_t) tau] < kYinThreshold)
        {
            // Found a candidate — but check if the next value is even lower
            // (we want the dip, not just the first value below threshold)
            while (tau + 1 < halfBuffer &&
                   yinBuffer[(size_t) (tau + 1)] < yinBuffer[(size_t) tau])
            {
                ++tau;
            }
            tauEstimate = tau;
            break;
        }
    }

    // If we didn't find anything below threshold, no pitch detected
    if (tauEstimate == -1)
    {
        detectedFrequency = 0.0f;
        detectedConfidence = 0.0f;
        return;
    }

    // ---- Step 4: Parabolic Interpolation ----
    // Refine the estimate by fitting a parabola around the best lag.
    // This gives sub-sample accuracy (important for tuning precision).
    float refinedTau = parabolicInterpolation (tauEstimate);

    // ---- Step 5: Convert lag to frequency ----
    // Frequency = sample_rate / period_in_samples
    if (refinedTau > 0.0f)
    {
        detectedFrequency = (float) currentSampleRate / refinedTau;
        detectedConfidence = 1.0f - yinBuffer[(size_t) tauEstimate];

        // Sanity check: human voice range is roughly 50 Hz to 2000 Hz
        // Musical range extends to about 4000 Hz
        if (detectedFrequency < 30.0f || detectedFrequency > 5000.0f)
        {
            detectedFrequency = 0.0f;
            detectedConfidence = 0.0f;
        }
    }
    else
    {
        detectedFrequency = 0.0f;
        detectedConfidence = 0.0f;
    }
}

float PitchDetector::parabolicInterpolation (int tauEstimate) const
{
    // Fit a parabola through three points:
    //   (tau-1, yinBuffer[tau-1])
    //   (tau,   yinBuffer[tau])
    //   (tau+1, yinBuffer[tau+1])
    // and find the x-value of the minimum.

    if (tauEstimate < 1 || tauEstimate >= kYinBufferSize - 1)
        return (float) tauEstimate;

    float s0 = yinBuffer[(size_t) (tauEstimate - 1)];
    float s1 = yinBuffer[(size_t) tauEstimate];
    float s2 = yinBuffer[(size_t) (tauEstimate + 1)];

    // The minimum of the parabola through these three points:
    float adjustment = (s2 - s0) / (2.0f * (2.0f * s1 - s2 - s0));

    return (float) tauEstimate + adjustment;
}
