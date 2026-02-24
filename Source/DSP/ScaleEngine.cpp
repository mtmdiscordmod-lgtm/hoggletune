#include "ScaleEngine.h"
#include <cmath>

// ============================================================================
// ScaleEngine Implementation
// ============================================================================

ScaleEngine::ScaleEngine()
{
}

void ScaleEngine::setRootNote (RootNote root)
{
    currentRoot = root;
}

void ScaleEngine::setMode (Mode mode)
{
    currentMode = mode;
}

float ScaleEngine::frequencyToMidi (float frequencyHz)
{
    // MIDI note formula: 69 + 12 * log2(freq / 440)
    // This gives us 69 for A4 (440 Hz), 60 for C4, etc.
    if (frequencyHz <= 0.0f)
        return 0.0f;

    return 69.0f + 12.0f * std::log2 (frequencyHz / 440.0f);
}

float ScaleEngine::midiToFrequency (float midiNote)
{
    // Reverse of the above: freq = 440 * 2^((midi - 69) / 12)
    return 440.0f * std::pow (2.0f, (midiNote - 69.0f) / 12.0f);
}

juce::String ScaleEngine::getNoteName (float frequencyHz)
{
    if (frequencyHz <= 0.0f)
        return "---";

    // Note names in order
    static const char* noteNames[] = {
        "C", "C#", "D", "D#", "E", "F",
        "F#", "G", "G#", "A", "A#", "B"
    };

    float midiNote = frequencyToMidi (frequencyHz);
    int roundedMidi = (int) std::round (midiNote);

    // MIDI note 0 = C-1, MIDI note 60 = C4
    int noteSemitone = ((roundedMidi % 12) + 12) % 12;  // Handle negatives
    int octave = (roundedMidi / 12) - 1;

    return juce::String (noteNames[noteSemitone]) + juce::String (octave);
}

bool ScaleEngine::isNoteInScale (int semitone) const
{
    // Adjust the semitone relative to the root note
    // For example, if root is D (2) and semitone is E (4),
    // the relative position is (4 - 2) = 2 semitones above root
    int relativeSemitone = ((semitone - (int) currentRoot) % 12 + 12) % 12;

    // Look up whether this relative position is in the scale pattern
    int modeIndex = (int) currentMode;
    return scalePatterns[(size_t) modeIndex][(size_t) relativeSemitone] == 1;
}

int ScaleEngine::findNearestScaleSemitone (int semitone) const
{
    // If this note is already in the scale, return it as-is
    if (isNoteInScale (semitone))
        return semitone;

    // Search outward from the given semitone to find the nearest scale note.
    // Check one semitone up, one down, two up, two down, etc.
    for (int offset = 1; offset <= 6; ++offset)
    {
        int up = (semitone + offset) % 12;
        int down = ((semitone - offset) % 12 + 12) % 12;

        bool upInScale = isNoteInScale (up);
        bool downInScale = isNoteInScale (down);

        if (upInScale && downInScale)
        {
            // Both are in scale at equal distance — prefer the lower one
            return down;
        }
        if (downInScale) return down;
        if (upInScale) return up;
    }

    // Shouldn't get here (chromatic includes everything), but just in case
    return semitone;
}

float ScaleEngine::getNearestScaleFrequency (float inputFrequencyHz) const
{
    if (inputFrequencyHz <= 0.0f)
        return 0.0f;

    // Step 1: Convert frequency to MIDI note (continuous, with decimals)
    float midiNote = frequencyToMidi (inputFrequencyHz);

    // Step 2: Round to nearest integer MIDI note to find which semitone we're on
    int nearestMidi = (int) std::round (midiNote);

    // Step 3: Get the semitone class (0-11) of this MIDI note
    int semitone = ((nearestMidi % 12) + 12) % 12;

    // Step 4: Find the nearest semitone that's in the current scale
    int targetSemitone = findNearestScaleSemitone (semitone);

    // Step 5: Calculate how many semitones we need to shift
    int shift = targetSemitone - semitone;

    // Handle wrapping (if target is more than 6 semitones away, go the other way)
    if (shift > 6) shift -= 12;
    if (shift < -6) shift += 12;

    // Step 6: The target MIDI note is the nearest integer MIDI note + the shift
    float targetMidi = (float) nearestMidi + (float) shift;

    // Step 7: Convert back to frequency
    return midiToFrequency (targetMidi);
}

float ScaleEngine::getSemitonesToNearest (float inputFrequencyHz) const
{
    if (inputFrequencyHz <= 0.0f)
        return 0.0f;

    float targetFreq = getNearestScaleFrequency (inputFrequencyHz);

    if (targetFreq <= 0.0f)
        return 0.0f;

    // The number of semitones between two frequencies:
    // semitones = 12 * log2(target / input)
    return 12.0f * std::log2 (targetFreq / inputFrequencyHz);
}
