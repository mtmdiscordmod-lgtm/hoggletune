#include "PluginEditor.h"

// ============================================================================
// PluginEditor Implementation — HoggleTune GUI
// ============================================================================

// Plugin window dimensions
static constexpr int kPluginWidth = 780;
static constexpr int kPluginHeight = 560;

HoggleTuneEditor::HoggleTuneEditor (HoggleTuneProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p)
{
    // Apply our custom vintage look to all child components
    setLookAndFeel (&skeuomorphicLnF);

    // ---- Title ----
    setupSectionLabel (titleLabel, "HOGGLETUNE");
    titleLabel.setFont (juce::Font (28.0f, juce::Font::bold));
    titleLabel.setColour (juce::Label::textColourId, SkeuomorphicLookAndFeel::kAccentGold);

    // ---- Pitch Correction Section ----
    setupSectionLabel (pitchCorrectionLabel, "PITCH CORRECTION");
    setupKnob (correctionSpeedKnob, correctionSpeedLabel, "Speed");

    // Root note dropdown (C, C#, D, D#, E, F, F#, G, G#, A, A#, B)
    rootNoteSelector.addItem ("C",  1);
    rootNoteSelector.addItem ("C#", 2);
    rootNoteSelector.addItem ("D",  3);
    rootNoteSelector.addItem ("D#", 4);
    rootNoteSelector.addItem ("E",  5);
    rootNoteSelector.addItem ("F",  6);
    rootNoteSelector.addItem ("F#", 7);
    rootNoteSelector.addItem ("G",  8);
    rootNoteSelector.addItem ("G#", 9);
    rootNoteSelector.addItem ("A",  10);
    rootNoteSelector.addItem ("A#", 11);
    rootNoteSelector.addItem ("B",  12);
    rootNoteSelector.setSelectedId (1);  // Default: C
    addAndMakeVisible (rootNoteSelector);

    // Scale mode dropdown
    scaleModeSelector.addItem ("Chromatic",   1);
    scaleModeSelector.addItem ("Ionian",      2);
    scaleModeSelector.addItem ("Dorian",      3);
    scaleModeSelector.addItem ("Phrygian",    4);
    scaleModeSelector.addItem ("Lydian",      5);
    scaleModeSelector.addItem ("Mixolydian",  6);
    scaleModeSelector.addItem ("Aeolian",     7);
    scaleModeSelector.addItem ("Locrian",     8);
    scaleModeSelector.setSelectedId (1);  // Default: Chromatic
    addAndMakeVisible (scaleModeSelector);

    // ---- Harmonizer Section ----
    setupSectionLabel (harmonizerLabel, "HARMONIZER");

    // Set up 4 voice controls
    const char* voiceEnabledParams[] = {
        HoggleTuneProcessor::PARAM_VOICE1_ENABLED,
        HoggleTuneProcessor::PARAM_VOICE2_ENABLED,
        HoggleTuneProcessor::PARAM_VOICE3_ENABLED,
        HoggleTuneProcessor::PARAM_VOICE4_ENABLED
    };
    const char* voiceIntervalParams[] = {
        HoggleTuneProcessor::PARAM_VOICE1_INTERVAL,
        HoggleTuneProcessor::PARAM_VOICE2_INTERVAL,
        HoggleTuneProcessor::PARAM_VOICE3_INTERVAL,
        HoggleTuneProcessor::PARAM_VOICE4_INTERVAL
    };
    const char* voiceGainParams[] = {
        HoggleTuneProcessor::PARAM_VOICE1_GAIN,
        HoggleTuneProcessor::PARAM_VOICE2_GAIN,
        HoggleTuneProcessor::PARAM_VOICE3_GAIN,
        HoggleTuneProcessor::PARAM_VOICE4_GAIN
    };

    for (int i = 0; i < 4; ++i)
    {
        auto& vc = voiceControls[(size_t) i];

        // Enable button
        vc.enableButton.setButtonText ("Voice " + juce::String (i + 1));
        addAndMakeVisible (vc.enableButton);

        // Interval knob
        setupKnob (vc.intervalKnob, voiceIntervalLabels[(size_t) i], "Interval");

        // Gain knob
        setupKnob (vc.gainKnob, voiceGainLabels[(size_t) i], "Gain");

        // Attachments
        voiceEnabledAttachments[(size_t) i] = std::make_unique<ButtonAttachment> (
            processorRef.apvts, voiceEnabledParams[i], vc.enableButton);
        voiceIntervalAttachments[(size_t) i] = std::make_unique<SliderAttachment> (
            processorRef.apvts, voiceIntervalParams[i], vc.intervalKnob);
        voiceGainAttachments[(size_t) i] = std::make_unique<SliderAttachment> (
            processorRef.apvts, voiceGainParams[i], vc.gainKnob);
    }

    // ---- Effects Section ----
    setupSectionLabel (effectsLabel, "EFFECTS");
    setupKnob (distortionKnob,    distortionLabel,    "Distortion");
    setupKnob (reverbAmountKnob,  reverbAmountLabel,  "Reverb");
    setupKnob (reverbSizeKnob,    reverbSizeLabel,    "Room Size");
    setupKnob (delayTimeKnob,     delayTimeLabel,     "Delay Time");
    setupKnob (delayFeedbackKnob, delayFeedbackLabel, "Feedback");
    setupKnob (delayMixKnob,      delayMixLabel,      "Delay Mix");

    // ---- Input/Output Section ----
    setupKnob (inputGainKnob,  inputGainLabel,  "Input");
    setupKnob (outputGainKnob, outputGainLabel, "Output");
    setupKnob (dryWetKnob,     dryWetLabel,     "Dry/Wet");

    // ---- APVTS Attachments ----
    // These automatically sync the GUI controls with the processor parameters
    correctionSpeedAttachment = std::make_unique<SliderAttachment> (
        processorRef.apvts, HoggleTuneProcessor::PARAM_CORRECTION_SPEED, correctionSpeedKnob);
    rootNoteAttachment = std::make_unique<ComboBoxAttachment> (
        processorRef.apvts, HoggleTuneProcessor::PARAM_ROOT_NOTE, rootNoteSelector);
    scaleModeAttachment = std::make_unique<ComboBoxAttachment> (
        processorRef.apvts, HoggleTuneProcessor::PARAM_SCALE_MODE, scaleModeSelector);

    distortionAttachment    = std::make_unique<SliderAttachment> (processorRef.apvts, HoggleTuneProcessor::PARAM_DISTORTION, distortionKnob);
    reverbAmountAttachment  = std::make_unique<SliderAttachment> (processorRef.apvts, HoggleTuneProcessor::PARAM_REVERB_AMOUNT, reverbAmountKnob);
    reverbSizeAttachment    = std::make_unique<SliderAttachment> (processorRef.apvts, HoggleTuneProcessor::PARAM_REVERB_SIZE, reverbSizeKnob);
    delayTimeAttachment     = std::make_unique<SliderAttachment> (processorRef.apvts, HoggleTuneProcessor::PARAM_DELAY_TIME, delayTimeKnob);
    delayFeedbackAttachment = std::make_unique<SliderAttachment> (processorRef.apvts, HoggleTuneProcessor::PARAM_DELAY_FEEDBACK, delayFeedbackKnob);
    delayMixAttachment      = std::make_unique<SliderAttachment> (processorRef.apvts, HoggleTuneProcessor::PARAM_DELAY_MIX, delayMixKnob);

    inputGainAttachment  = std::make_unique<SliderAttachment> (processorRef.apvts, HoggleTuneProcessor::PARAM_INPUT_GAIN, inputGainKnob);
    outputGainAttachment = std::make_unique<SliderAttachment> (processorRef.apvts, HoggleTuneProcessor::PARAM_OUTPUT_GAIN, outputGainKnob);
    dryWetAttachment     = std::make_unique<SliderAttachment> (processorRef.apvts, HoggleTuneProcessor::PARAM_DRY_WET_MIX, dryWetKnob);

    // Start timer for pitch display updates (30 fps)
    startTimerHz (30);

    // Set the plugin window size
    setSize (kPluginWidth, kPluginHeight);
}

HoggleTuneEditor::~HoggleTuneEditor()
{
    setLookAndFeel (nullptr);
}

// ============================================================================
// Paint — Draw the background and static elements
// ============================================================================

void HoggleTuneEditor::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Draw brushed metal background
    SkeuomorphicLookAndFeel::drawBrushedMetal (g, bounds);

    // ---- Header area ----
    auto headerArea = bounds.removeFromTop (60);

    // Draw brand line under title
    g.setColour (SkeuomorphicLookAndFeel::kAccentGold.withAlpha (0.5f));
    g.drawLine (10, 55, (float) getWidth() - 10, 55, 1.0f);

    // ---- Pitch display (LCD in header) ----
    auto lcdArea = headerArea.removeFromRight (200).reduced (10, 10);
    SkeuomorphicLookAndFeel::drawLcdDisplay (g, lcdArea, currentNoteText, currentPitchText);

    // ---- Section panels ----
    int y = 65;

    // Pitch Correction panel
    SkeuomorphicLookAndFeel::drawRecessedPanel (g, { 10, y, 230, 180 });

    // Harmonizer panel
    SkeuomorphicLookAndFeel::drawRecessedPanel (g, { 250, y, 520, 180 });

    y += 190;

    // Effects panel
    SkeuomorphicLookAndFeel::drawRecessedPanel (g, { 10, y, 760, 145 });

    y += 155;

    // I/O panel
    SkeuomorphicLookAndFeel::drawRecessedPanel (g, { 10, y, 760, 100 });
}

// ============================================================================
// Resized — Position all GUI components
// ============================================================================

void HoggleTuneEditor::resized()
{
    int knobSize = 65;
    int knobSpacing = 10;

    // ---- Header ----
    titleLabel.setBounds (15, 10, 300, 40);

    // ---- Pitch Correction Section (left side, row 1) ----
    int pcY = 90;
    pitchCorrectionLabel.setBounds (20, 68, 200, 20);

    correctionSpeedKnob.setBounds (25, pcY, knobSize, knobSize);
    correctionSpeedLabel.setBounds (25, pcY + knobSize, knobSize, 18);

    rootNoteSelector.setBounds  (100, pcY + 5,  120, 28);
    scaleModeSelector.setBounds (100, pcY + 40, 120, 28);

    // ---- Harmonizer Section (right side, row 1) ----
    harmonizerLabel.setBounds (260, 68, 200, 20);

    int hStartX = 265;
    int voiceWidth = 120;

    for (int i = 0; i < 4; ++i)
    {
        auto& vc = voiceControls[(size_t) i];
        int vx = hStartX + i * voiceWidth;

        vc.enableButton.setBounds  (vx, pcY - 2, voiceWidth - 5, 24);

        vc.intervalKnob.setBounds  (vx, pcY + 26, knobSize - 10, knobSize - 10);
        voiceIntervalLabels[(size_t) i].setBounds (vx, pcY + 26 + knobSize - 10, knobSize - 10, 16);

        vc.gainKnob.setBounds      (vx + knobSize - 5, pcY + 26, knobSize - 10, knobSize - 10);
        voiceGainLabels[(size_t) i].setBounds (vx + knobSize - 5, pcY + 26 + knobSize - 10, knobSize - 10, 16);
    }

    // ---- Effects Section (row 2) ----
    int fxY = 280;
    effectsLabel.setBounds (20, 258, 200, 20);

    int fxKnobSize = knobSize;
    int fxStartX = 30;
    int fxSpacing = knobSize + 30 + knobSpacing;

    distortionKnob.setBounds    (fxStartX,                fxY, fxKnobSize, fxKnobSize);
    distortionLabel.setBounds   (fxStartX,                fxY + fxKnobSize, fxKnobSize + 20, 18);
    reverbAmountKnob.setBounds  (fxStartX + fxSpacing,    fxY, fxKnobSize, fxKnobSize);
    reverbAmountLabel.setBounds (fxStartX + fxSpacing,    fxY + fxKnobSize, fxKnobSize + 20, 18);
    reverbSizeKnob.setBounds    (fxStartX + fxSpacing*2,  fxY, fxKnobSize, fxKnobSize);
    reverbSizeLabel.setBounds   (fxStartX + fxSpacing*2,  fxY + fxKnobSize, fxKnobSize + 20, 18);
    delayTimeKnob.setBounds     (fxStartX + fxSpacing*3,  fxY, fxKnobSize, fxKnobSize);
    delayTimeLabel.setBounds    (fxStartX + fxSpacing*3,  fxY + fxKnobSize, fxKnobSize + 20, 18);
    delayFeedbackKnob.setBounds (fxStartX + fxSpacing*4,  fxY, fxKnobSize, fxKnobSize);
    delayFeedbackLabel.setBounds(fxStartX + fxSpacing*4,  fxY + fxKnobSize, fxKnobSize + 20, 18);
    delayMixKnob.setBounds      (fxStartX + fxSpacing*5,  fxY, fxKnobSize, fxKnobSize);
    delayMixLabel.setBounds     (fxStartX + fxSpacing*5,  fxY + fxKnobSize, fxKnobSize + 20, 18);

    // ---- I/O Section (row 3) ----
    int ioY = 430;
    int ioKnobSize = knobSize;
    int ioSpacing = 250;

    inputGainKnob.setBounds  (50,               ioY, ioKnobSize, ioKnobSize);
    inputGainLabel.setBounds (50,               ioY + ioKnobSize, ioKnobSize + 20, 18);
    dryWetKnob.setBounds     (50 + ioSpacing,   ioY, ioKnobSize, ioKnobSize);
    dryWetLabel.setBounds    (50 + ioSpacing,   ioY + ioKnobSize, ioKnobSize + 20, 18);
    outputGainKnob.setBounds (50 + ioSpacing*2, ioY, ioKnobSize, ioKnobSize);
    outputGainLabel.setBounds(50 + ioSpacing*2, ioY + ioKnobSize, ioKnobSize + 20, 18);
}

// ============================================================================
// Timer Callback — Update pitch display at 30fps
// ============================================================================

void HoggleTuneEditor::timerCallback()
{
    float pitch = processorRef.getDetectedPitch();

    if (pitch > 0.0f)
    {
        currentNoteText = processorRef.getDetectedNoteName();
        currentPitchText = juce::String (pitch, 1) + " Hz";
    }
    else
    {
        currentNoteText = "---";
        currentPitchText = "---";
    }

    repaint (getWidth() - 210, 0, 210, 60);  // Only repaint the LCD area
}

// ============================================================================
// Helper Methods
// ============================================================================

void HoggleTuneEditor::setupKnob (juce::Slider& knob, juce::Label& label,
                                    const juce::String& labelText)
{
    knob.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    knob.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 16);
    addAndMakeVisible (knob);

    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setFont (juce::Font (11.0f));
    label.setColour (juce::Label::textColourId, SkeuomorphicLookAndFeel::kTextDim);
    addAndMakeVisible (label);
}

void HoggleTuneEditor::setupSectionLabel (juce::Label& label, const juce::String& text)
{
    label.setText (text, juce::dontSendNotification);
    label.setFont (juce::Font (14.0f, juce::Font::bold));
    label.setColour (juce::Label::textColourId, SkeuomorphicLookAndFeel::kAccentGold);
    label.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (label);
}
