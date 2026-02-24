#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "GUI/SkeuomorphicLookAndFeel.h"

// ============================================================================
// PluginEditor — The HoggleTune GUI
// ============================================================================
// This is the visual interface users see when they open the plugin in their DAW.
// It's organized into sections:
//
// ┌─────────────────────────────────────────────────────────────┐
// │  HOGGLETUNE                              [Pitch Display]   │
// ├──────────────────────┬──────────────────────────────────────┤
// │  PITCH CORRECTION    │  HARMONIZER                         │
// │  Speed  Key  Mode    │  Voice1  Voice2  Voice3  Voice4     │
// │   (O)  [C] [Major]   │  [on]    [on]    [on]    [on]      │
// │                      │  Int:(O) Int:(O) Int:(O) Int:(O)    │
// │                      │  Vol:(O) Vol:(O) Vol:(O) Vol:(O)    │
// ├──────────────────────┴──────────────────────────────────────┤
// │  EFFECTS                                                    │
// │  Distort  Reverb  RevSize  DelTime  DelFB  DelMix          │
// │    (O)     (O)     (O)      (O)     (O)     (O)            │
// ├─────────────────────────────────────────────────────────────┤
// │  INPUT (O)          DRY/WET (O)          OUTPUT (O)        │
// └─────────────────────────────────────────────────────────────┘
// ============================================================================

class HoggleTuneEditor : public juce::AudioProcessorEditor,
                          private juce::Timer
{
public:
    explicit HoggleTuneEditor (HoggleTuneProcessor& processor);
    ~HoggleTuneEditor() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    // Timer callback — updates the pitch display
    void timerCallback() override;

    // Reference to the processor (for reading pitch data and parameters)
    HoggleTuneProcessor& processorRef;

    // Custom look and feel
    SkeuomorphicLookAndFeel skeuomorphicLnF;

    // ---- Pitch Correction Controls ----
    juce::Slider correctionSpeedKnob;
    juce::ComboBox rootNoteSelector;
    juce::ComboBox scaleModeSelector;

    // ---- Harmonizer Voice Controls ----
    struct VoiceControls
    {
        juce::ToggleButton enableButton;
        juce::Slider intervalKnob;
        juce::Slider gainKnob;
    };
    std::array<VoiceControls, 4> voiceControls;

    // ---- Effects Controls ----
    juce::Slider distortionKnob;
    juce::Slider reverbAmountKnob;
    juce::Slider reverbSizeKnob;
    juce::Slider delayTimeKnob;
    juce::Slider delayFeedbackKnob;
    juce::Slider delayMixKnob;

    // ---- Input/Output Controls ----
    juce::Slider inputGainKnob;
    juce::Slider outputGainKnob;
    juce::Slider dryWetKnob;

    // ---- Labels ----
    juce::Label titleLabel;
    juce::Label pitchCorrectionLabel;
    juce::Label harmonizerLabel;
    juce::Label effectsLabel;

    // Per-knob labels
    juce::Label correctionSpeedLabel;
    juce::Label distortionLabel;
    juce::Label reverbAmountLabel;
    juce::Label reverbSizeLabel;
    juce::Label delayTimeLabel;
    juce::Label delayFeedbackLabel;
    juce::Label delayMixLabel;
    juce::Label inputGainLabel;
    juce::Label outputGainLabel;
    juce::Label dryWetLabel;

    // Harmony voice labels
    std::array<juce::Label, 4> voiceIntervalLabels;
    std::array<juce::Label, 4> voiceGainLabels;

    // ---- APVTS Attachments ----
    // These connect GUI controls to processor parameters automatically
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> correctionSpeedAttachment;
    std::unique_ptr<ComboBoxAttachment> rootNoteAttachment;
    std::unique_ptr<ComboBoxAttachment> scaleModeAttachment;

    std::array<std::unique_ptr<ButtonAttachment>, 4> voiceEnabledAttachments;
    std::array<std::unique_ptr<SliderAttachment>, 4> voiceIntervalAttachments;
    std::array<std::unique_ptr<SliderAttachment>, 4> voiceGainAttachments;

    std::unique_ptr<SliderAttachment> distortionAttachment;
    std::unique_ptr<SliderAttachment> reverbAmountAttachment;
    std::unique_ptr<SliderAttachment> reverbSizeAttachment;
    std::unique_ptr<SliderAttachment> delayTimeAttachment;
    std::unique_ptr<SliderAttachment> delayFeedbackAttachment;
    std::unique_ptr<SliderAttachment> delayMixAttachment;

    std::unique_ptr<SliderAttachment> inputGainAttachment;
    std::unique_ptr<SliderAttachment> outputGainAttachment;
    std::unique_ptr<SliderAttachment> dryWetAttachment;

    // ---- Helper ----
    void setupKnob (juce::Slider& knob, juce::Label& label, const juce::String& labelText);
    void setupSectionLabel (juce::Label& label, const juce::String& text);

    // Pitch readout
    juce::String currentPitchText = "---";
    juce::String currentNoteText = "---";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HoggleTuneEditor)
};
