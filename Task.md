<!-- hoggle-memory-system: v1.0 -->
# Task.md

## Current Task: Build Core HoggleTune Plugin

**Status:** In Progress
**Started:** 2026-02-24
**Last Updated:** 2026-02-24
**Context:** Initial build of the HoggleTune autotune/harmonizer plugin. Setting up the full project structure, DSP engine, and GUI.

---

### Priority Order

### 1. Project Foundation [IN PROGRESS]
**Why first:** Everything depends on the build system and project structure.
**Parent component:** CMake Build

- [x] Set up CMakeLists.txt with JUCE FetchContent
- [x] Create directory structure (Source/DSP, Source/GUI)
- [x] Create project documentation files

### 2. Core DSP Engine [IN PROGRESS]
**Why second:** The DSP is the heart of the plugin — GUI wraps around it.
**Parent component:** All DSP components

- [x] PitchDetector (YIN algorithm)
- [x] ScaleEngine (chromatic + 7 diatonic modes)
- [x] PitchShifter (circular buffer with crossfade)
- [x] AutotuneEngine (pitch correction with speed control)
- [x] HarmonizerEngine (4 voices, selectable intervals)
- [x] DistortionProcessor (soft-clip, default <10%)
- [x] ReverbProcessor (JUCE Reverb wrapper, default <10%)
- [x] DelayProcessor (delay line, default <6% wet)

### 3. Plugin Integration [IN PROGRESS]
**Why third:** Connects DSP to the host (DAW).
**Parent component:** PluginProcessor

- [x] PluginProcessor with all parameters
- [x] Audio processing chain
- [x] Parameter management (AudioProcessorValueTreeState)

### 4. Skeuomorphic GUI [IN PROGRESS]
**Why fourth:** Visual layer on top of the working DSP.
**Parent component:** PluginEditor

- [x] SkeuomorphicLookAndFeel (custom knobs, sliders, backgrounds)
- [x] PluginEditor layout with all controls
- [x] Pitch display (detected pitch readout)

---

### What's Next

- Test compilation on macOS/Windows/Linux
- Test in a DAW (Logic Pro, Ableton, Pro Tools)
- Fine-tune pitch detection accuracy
- Polish GUI graphics (replace procedural drawing with actual textures if desired)
- Add preset system

---

### Notes

- Developer is a beginner with C++/DSP — all code should be heavily commented
- Default FX levels: distortion <10%, reverb <10%, delay <6% wet
- Harmonizer interval range: -24 to +24 semitones per voice
- Correction speed: 0% (no correction) to 100% (instant snap)
