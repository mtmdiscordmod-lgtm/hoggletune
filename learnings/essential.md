<!-- hoggle-memory-system: v1.0 -->
# Essential Learnings

> **Read this EVERY session.** Distilled top learnings from working on this project.
>
> **Last updated:** 2026-02-24
>
> **Target:** Keep this file under ~80 lines.

---

## How the Developer Works

- Beginner with C++ and audio DSP — explain code thoroughly with comments
- Strong in music production and creative direction
- Prefers seeing the big picture before diving into implementation details
- Skeuomorphic UI preference (vintage hardware look)

## Core Architecture (Don't Re-Litigate)

- We use JUCE 7 for the plugin framework. Not rolling our own AU/VST wrapper.
- YIN algorithm for pitch detection. Not autocorrelation or FFT-based.
- Circular buffer with dual read heads for pitch shifting. Not phase vocoder (too complex for v1).
- CMake build system with FetchContent for JUCE. Not Projucer.
- Plugin formats: AU + VST3 + AAX.

## Critical Mistakes to Avoid

- (This section starts empty. Add entries as mistakes happen.)

## Proven Patterns

- (This section starts empty. Add entries as patterns emerge.)

## Outdated Lessons

- (This section starts empty.)
