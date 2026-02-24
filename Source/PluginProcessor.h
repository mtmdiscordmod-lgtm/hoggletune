#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#include "DSP/AutotuneEngine.h"
#include "DSP/HarmonizerEngine.h"
#include "DSP/DistortionProcessor.h"
#include "DSP/ReverbProcessor.h"
#include "DSP/DelayProcessor.h"

// ============================================================================
// PluginProcessor — The brain of HoggleTune
// ============================================================================
// This is the main class that the DAW (Logic, Ableton, Pro Tools, etc.) talks to.
// It owns all the DSP components and routes audio through them.
//
// Audio flow:
//   Input → Autotune → Harmonizer (mixed in) → Distortion → Reverb → Delay → Output
//
// All user-adjustable parameters are managed by AudioProcessorValueTreeState (APVTS),
// which provides thread-safe parameter access and automatic GUI synchronization.
// ============================================================================

class HoggleTuneProcessor : public juce::AudioProcessor
{
public:
    HoggleTuneProcessor();
    ~HoggleTuneProcessor() override;

    // ---- JUCE AudioProcessor interface ----
    // These are required methods that every JUCE plugin must implement.

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // Plugin metadata
    const juce::String getName() const override { return "HoggleTune"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }  // Reverb/delay tail

    // Preset/program support (basic implementation)
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    // State save/load (for DAW session recall)
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Editor (GUI)
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    // ---- Parameter Tree ----
    // APVTS manages all parameters. The GUI attaches to this.
    juce::AudioProcessorValueTreeState apvts;

    // ---- Readouts for the GUI ----
    float getDetectedPitch() const { return autotuneEngine.getDetectedPitch(); }
    float getTargetPitch() const { return autotuneEngine.getTargetPitch(); }
    juce::String getDetectedNoteName() const { return autotuneEngine.getDetectedNoteName(); }

    // ---- Parameter ID constants ----
    // These string IDs connect parameters between the processor and the GUI.
    // Using constexpr ensures they're consistent everywhere.
    static constexpr const char* PARAM_CORRECTION_SPEED = "correctionSpeed";
    static constexpr const char* PARAM_ROOT_NOTE        = "rootNote";
    static constexpr const char* PARAM_SCALE_MODE       = "scaleMode";
    static constexpr const char* PARAM_INPUT_GAIN       = "inputGain";
    static constexpr const char* PARAM_OUTPUT_GAIN      = "outputGain";
    static constexpr const char* PARAM_DRY_WET_MIX      = "dryWetMix";

    // Harmonizer voice parameters (voiceN prefix, N = 1-4)
    static constexpr const char* PARAM_VOICE1_ENABLED   = "voice1Enabled";
    static constexpr const char* PARAM_VOICE1_INTERVAL  = "voice1Interval";
    static constexpr const char* PARAM_VOICE1_GAIN      = "voice1Gain";
    static constexpr const char* PARAM_VOICE2_ENABLED   = "voice2Enabled";
    static constexpr const char* PARAM_VOICE2_INTERVAL  = "voice2Interval";
    static constexpr const char* PARAM_VOICE2_GAIN      = "voice2Gain";
    static constexpr const char* PARAM_VOICE3_ENABLED   = "voice3Enabled";
    static constexpr const char* PARAM_VOICE3_INTERVAL  = "voice3Interval";
    static constexpr const char* PARAM_VOICE3_GAIN      = "voice3Gain";
    static constexpr const char* PARAM_VOICE4_ENABLED   = "voice4Enabled";
    static constexpr const char* PARAM_VOICE4_INTERVAL  = "voice4Interval";
    static constexpr const char* PARAM_VOICE4_GAIN      = "voice4Gain";

    // Effects parameters
    static constexpr const char* PARAM_DISTORTION       = "distortion";
    static constexpr const char* PARAM_REVERB_AMOUNT    = "reverbAmount";
    static constexpr const char* PARAM_REVERB_SIZE      = "reverbSize";
    static constexpr const char* PARAM_DELAY_TIME       = "delayTime";
    static constexpr const char* PARAM_DELAY_FEEDBACK   = "delayFeedback";
    static constexpr const char* PARAM_DELAY_MIX        = "delayMix";

private:
    // Create the parameter layout (called once in the constructor)
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Read all parameters from APVTS and apply them to DSP components
    void updateParameters();

    // ---- DSP Components ----
    AutotuneEngine autotuneEngine;
    HarmonizerEngine harmonizerEngine;
    DistortionProcessor distortionProcessor;
    ReverbProcessor reverbProcessor;
    DelayProcessor delayProcessor;

    // Temporary buffer for mono processing
    std::vector<float> monoBuffer;
    std::vector<float> harmonyBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HoggleTuneProcessor)
};
