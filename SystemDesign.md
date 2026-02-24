<!-- hoggle-memory-system: v1.0 -->
# SystemDesign.md

> **For small/new projects**, this single file is fine. As it grows past ~500 lines, split into `docs/architecture/` using the routed structure (see Architecture Routing below).

## Overview

HoggleTune is a real-time audio plugin that provides pitch correction (autotune) and harmonization. It accepts mono or stereo audio input, detects pitch using the YIN algorithm, corrects pitch to a user-selected scale/mode, generates up to 4 harmony voices, and applies built-in effects (distortion, reverb, delay). Built with JUCE for AU/VST3/AAX compatibility.

**Current state:** Initial implementation — all core components being built.

---

## Architecture Diagram

```
                        ┌─────────────────────────────────────────────────┐
                        │              HoggleTune Plugin                  │
                        │                                                 │
  Audio In ────────────>│  ┌──────────────┐    ┌───────────────────┐      │
  (Mono/Stereo)         │  │ PitchDetector│    │   ScaleEngine     │      │
                        │  │   (YIN)      │───>│ (modes, keys,     │      │
                        │  └──────┬───────┘    │  nearest pitch)   │      │
                        │         │            └─────────┬─────────┘      │
                        │         v                      │                │
                        │  ┌──────────────┐              │                │
                        │  │ AutotuneEngine│<────────────┘                │
                        │  │ (pitch shift  │                              │
                        │  │  to target)   │                              │
                        │  └──────┬───────┘                               │
                        │         │                                       │
                        │         ├──────────────────────┐                │
                        │         v                      v                │
                        │  ┌──────────────┐    ┌─────────────────┐        │
                        │  │  Corrected   │    │ HarmonizerEngine│        │
                        │  │  Signal      │    │ (4 voices,      │        │
                        │  │              │    │  pitch shifted)  │        │
                        │  └──────┬───────┘    └────────┬────────┘        │
                        │         │                     │                 │
                        │         v                     v                 │
                        │  ┌──────────────────────────────────────┐       │
                        │  │           Mix Bus                    │       │
                        │  └──────────────┬──────────────────────┘       │
                        │                 │                               │
                        │                 v                               │
                        │  ┌──────────┐ ┌──────────┐ ┌──────────┐        │
                        │  │Distortion│>│  Reverb   │>│  Delay   │        │
                        │  │ (<10%)   │ │  (<10%)   │ │  (<6%)   │        │
                        │  └──────────┘ └──────────┘ └──────────┘        │
                        │                 │                               │
                        └─────────────────┼───────────────────────────────┘
                                          v
                                     Audio Out
```

---

## Component Details

### 1. PitchDetector (YIN Algorithm)

**Purpose:** Detects the fundamental frequency (pitch) of the incoming audio in real-time.
**Key files:** `Source/DSP/PitchDetector.h`, `Source/DSP/PitchDetector.cpp`
**Dependencies:** None (standalone DSP)

Uses the YIN algorithm which computes a difference function, normalizes it, then finds the best pitch period. Outputs frequency in Hz. Runs on a mono sum of the input.

### 2. ScaleEngine

**Purpose:** Maps detected pitches to the nearest "correct" pitch based on the selected key and mode.
**Key files:** `Source/DSP/ScaleEngine.h`, `Source/DSP/ScaleEngine.cpp`
**Dependencies:** None

Supports chromatic mode (all 12 semitones) and 7 diatonic modes (Ionian through Locrian). Given a detected frequency, returns the nearest target frequency in the selected scale.

### 3. AutotuneEngine

**Purpose:** Applies pitch correction by shifting the input audio toward the target pitch.
**Key files:** `Source/DSP/AutotuneEngine.h`, `Source/DSP/AutotuneEngine.cpp`
**Dependencies:** PitchDetector, ScaleEngine, PitchShifter

Calculates the semitone difference between detected and target pitch, then uses PitchShifter to shift the audio. Correction speed parameter controls how quickly pitch snaps to target (0% = no correction, 100% = instant snap).

### 4. PitchShifter

**Purpose:** Shifts audio pitch by a specified number of semitones in real-time.
**Key files:** `Source/DSP/PitchShifter.h`, `Source/DSP/PitchShifter.cpp`
**Dependencies:** None (standalone DSP)

Uses a dual-read-head circular buffer with crossfading. Handles small and large pitch shifts for both autotune and harmonizer use cases.

### 5. HarmonizerEngine

**Purpose:** Generates up to 4 additional harmony voices at user-selected intervals.
**Key files:** `Source/DSP/HarmonizerEngine.h`, `Source/DSP/HarmonizerEngine.cpp`
**Dependencies:** PitchShifter

Each voice is an independent PitchShifter set to a different interval (in semitones). Voices can be individually enabled/disabled and have independent volume controls.

### 6. Effects Chain (Distortion, Reverb, Delay)

**Purpose:** Built-in effects applied after pitch processing.
**Key files:** `Source/DSP/DistortionProcessor.h/.cpp`, `Source/DSP/ReverbProcessor.h/.cpp`, `Source/DSP/DelayProcessor.h/.cpp`
**Dependencies:** None (standalone DSP, uses JUCE dsp module for reverb)

- Distortion: Soft-clip waveshaping, default <10%
- Reverb: Uses JUCE's built-in Reverb class, default <10%
- Delay: Simple delay line with feedback, default <6% wet

### 7. Plugin Processor

**Purpose:** Main JUCE plugin host — owns all DSP components, manages parameters, processes audio.
**Key files:** `Source/PluginProcessor.h`, `Source/PluginProcessor.cpp`
**Dependencies:** All DSP components

Uses `juce::AudioProcessorValueTreeState` for parameter management. Coordinates the entire audio processing chain.

### 8. Plugin Editor (Skeuomorphic GUI)

**Purpose:** Visual interface with vintage hardware aesthetic.
**Key files:** `Source/PluginEditor.h`, `Source/PluginEditor.cpp`, `Source/GUI/SkeuomorphicLookAndFeel.h/.cpp`
**Dependencies:** Plugin Processor (for parameter access)

Custom look-and-feel with brushed metal backgrounds, backlit displays, beveled knobs.

---

## Key Contracts

### Parameter IDs
- **Between:** PluginProcessor and PluginEditor
- **Format:** String parameter IDs defined in PluginProcessor.h
- **Schema:** All parameter IDs are `constexpr` strings (e.g., `"correctionSpeed"`, `"rootNote"`, `"scaleMode"`)

### Pitch Data Flow
- **Between:** PitchDetector and AutotuneEngine
- **Format:** `float` frequency in Hz (0.0 = no pitch detected)

---

## Known Pitfalls

- **Buffer size handling:** The YIN algorithm needs a minimum buffer size (~2048 samples) for accurate low-frequency detection. Input buffers may be smaller, so internal buffering is needed.
- **Crossfade artifacts:** The dual-read-head pitch shifter can produce artifacts at crossfade points if the crossfade window is too short.
- **Thread safety:** Parameter changes come from the GUI thread; audio processing is on the audio thread. Use `std::atomic` for parameter values.

---

## Status Tracker

| Component | Status | Notes |
|-----------|--------|-------|
| PitchDetector | IN PROGRESS | YIN algorithm implementation |
| ScaleEngine | IN PROGRESS | All modes + chromatic |
| AutotuneEngine | IN PROGRESS | Core pitch correction |
| PitchShifter | IN PROGRESS | Circular buffer approach |
| HarmonizerEngine | IN PROGRESS | 4-voice harmonizer |
| DistortionProcessor | IN PROGRESS | Soft-clip waveshaping |
| ReverbProcessor | IN PROGRESS | JUCE Reverb wrapper |
| DelayProcessor | IN PROGRESS | Simple delay line |
| PluginProcessor | IN PROGRESS | Parameter setup + audio chain |
| PluginEditor | IN PROGRESS | Skeuomorphic GUI |
| CMake Build | IN PROGRESS | JUCE + CMake setup |

---

## Configuration

| Config File | Purpose | Sensitive? |
|-------------|---------|------------|
| `.env` | Build-time secrets (signing certs, etc.) | YES — gitignored |
| `CMakeLists.txt` | Build configuration | No |

---

## Dependencies

| Dependency | Purpose | Why This One |
|------------|---------|--------------|
| JUCE 7 | Audio plugin framework | Industry standard, supports AU/VST3/AAX, excellent DSP utilities |
| CMake 3.22+ | Build system | JUCE's recommended build system, cross-platform |

---

## Architecture Routing

When this file grows past ~500 lines, split into a routed structure:

```
docs/
  architecture/
    INDEX.md           <- component-to-doc mapping table (always read)
    overview.md        <- this file's Overview + Diagram + Status Tracker (always read)
    [component].md     <- one per major subsystem (read only when relevant)
```
