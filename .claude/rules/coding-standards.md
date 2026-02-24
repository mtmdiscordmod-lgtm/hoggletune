# Coding Standards — HoggleTune

## C++ Style
- Use `camelCase` for variables and functions, `PascalCase` for classes
- JUCE naming conventions: prefix member variables with nothing (JUCE style), not `m_`
- Use `auto` sparingly — prefer explicit types for clarity (beginner-friendly codebase)
- Every function and class gets a comment explaining what it does in plain English
- Use `juce::` namespace prefix explicitly (don't use `using namespace juce`)

## Real-Time Audio Safety
- NEVER allocate memory (new, malloc, vector resize) in `processBlock()` or any function called from it
- NEVER use locks/mutexes in the audio thread — use `std::atomic` for thread-safe parameter access
- NEVER throw exceptions in the audio thread
- Pre-allocate all buffers in `prepareToPlay()`

## JUCE Conventions
- Use `juce::AudioProcessorValueTreeState` for all plugin parameters
- Parameter IDs are lowercase with no spaces (e.g., `"correctionSpeed"`, `"rootNote"`)
- Use `juce::dsp` module classes where available (Reverb, Gain, etc.)

## File Organization
- One class per header/source pair
- DSP classes go in `Source/DSP/`
- GUI classes go in `Source/GUI/`
- Plugin entry points (Processor/Editor) go in `Source/`
