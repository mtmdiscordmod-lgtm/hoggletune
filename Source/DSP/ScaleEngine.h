#pragma once

#include <juce_core/juce_core.h>
#include <array>

// ============================================================================
// ScaleEngine — Maps frequencies to the nearest note in a musical scale
// ============================================================================
// Given a detected pitch frequency, this engine figures out what the nearest
// "correct" note should be, based on the user's selected key and mode.
//
// Musical background:
// - There are 12 notes in Western music: C, C#, D, D#, E, F, F#, G, G#, A, A#, B
// - A "mode" is a subset of these 12 notes (7 notes selected from the 12)
// - Each mode has a different pattern of whole and half steps
// - "Chromatic" mode uses all 12 notes (any note is considered "correct")
//
// The 7 diatonic modes (using C as root for example):
//   Ionian (Major):     C D E F G A B     — happy, bright
//   Dorian:             C D Eb F G A Bb   — jazzy, minor-ish
//   Phrygian:           C Db Eb F G Ab Bb — dark, Spanish
//   Lydian:             C D E F# G A B    — dreamy, floating
//   Mixolydian:         C D E F G A Bb    — bluesy, rock
//   Aeolian (Minor):    C D Eb F G Ab Bb  — sad, dark
//   Locrian:            C Db Eb F Gb Ab Bb — unstable, dissonant
// ============================================================================

class ScaleEngine
{
public:
    // All available scale modes
    enum class Mode
    {
        Chromatic = 0,  // All 12 notes
        Ionian,         // Major scale
        Dorian,
        Phrygian,
        Lydian,
        Mixolydian,
        Aeolian,        // Natural minor scale
        Locrian,

        NumModes
    };

    // Root notes (0 = C, 1 = C#, ... 11 = B)
    enum class RootNote
    {
        C = 0, CSharp, D, DSharp, E, F,
        FSharp, G, GSharp, A, ASharp, B,

        NumNotes
    };

    ScaleEngine();
    ~ScaleEngine() = default;

    // Set the current key (root note) and mode
    void setRootNote (RootNote root);
    void setMode (Mode mode);

    // Given a frequency in Hz, return the nearest frequency that's "in tune"
    // with the current scale. This is the target the autotune aims for.
    float getNearestScaleFrequency (float inputFrequencyHz) const;

    // Get the semitone shift needed to move from the input frequency
    // to the nearest scale frequency. Positive = shift up, negative = shift down.
    float getSemitonesToNearest (float inputFrequencyHz) const;

    // Utility: convert frequency to MIDI note number (69 = A4 = 440 Hz)
    static float frequencyToMidi (float frequencyHz);

    // Utility: convert MIDI note number to frequency
    static float midiToFrequency (float midiNote);

    // Get the name of a note closest to the given frequency (e.g., "C4", "A#3")
    static juce::String getNoteName (float frequencyHz);

private:
    // Check if a given semitone (0-11) is in the current scale
    bool isNoteInScale (int semitone) const;

    // Find the nearest semitone in the scale to the given semitone
    int findNearestScaleSemitone (int semitone) const;

    RootNote currentRoot = RootNote::C;
    Mode currentMode = Mode::Chromatic;

    // Scale patterns: each mode is defined by which of the 12 semitones
    // are included, relative to the root. 1 = included, 0 = excluded.
    // Index 0 = root, index 1 = minor 2nd, ... index 11 = major 7th
    static constexpr std::array<std::array<int, 12>, 8> scalePatterns = {{
        // Chromatic: all 12 notes
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        // Ionian (Major): W W H W W W H  (whole/half step pattern)
        {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1},
        // Dorian: W H W W W H W
        {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0},
        // Phrygian: H W W W H W W
        {1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
        // Lydian: W W W H W W H
        {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
        // Mixolydian: W W H W W H W
        {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0},
        // Aeolian (Minor): W H W W H W W
        {1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0},
        // Locrian: H W W H W W W
        {1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0}
    }};
};
