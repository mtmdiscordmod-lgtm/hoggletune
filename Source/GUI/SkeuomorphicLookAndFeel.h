#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

// ============================================================================
// SkeuomorphicLookAndFeel — Vintage hardware visual style
// ============================================================================
// JUCE's LookAndFeel system lets us customize how every GUI component is drawn.
// This class overrides the default drawing methods to create a vintage
// hardware rack unit aesthetic with:
// - Brushed metal background
// - Beveled, 3D-looking knobs with pointer indicators
// - Backlit LCD-style text displays
// - Warm color scheme (golds, ambers, dark grays)
// ============================================================================

class SkeuomorphicLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SkeuomorphicLookAndFeel();
    ~SkeuomorphicLookAndFeel() override = default;

    // ---- Color constants for the vintage theme ----
    static inline const juce::Colour kBackgroundDark   { 0xFF1A1A2E };  // Dark navy
    static inline const juce::Colour kBackgroundMid    { 0xFF16213E };  // Mid navy
    static inline const juce::Colour kPanelColor       { 0xFF2C2C3E };  // Panel gray
    static inline const juce::Colour kKnobBody         { 0xFF3A3A4A };  // Knob body
    static inline const juce::Colour kKnobHighlight    { 0xFF5A5A6A };  // Knob edge highlight
    static inline const juce::Colour kAccentGold       { 0xFFE2B714 };  // Gold accent
    static inline const juce::Colour kAccentAmber      { 0xFFFF8C00 };  // Amber/orange
    static inline const juce::Colour kTextLight        { 0xFFE0E0E0 };  // Light text
    static inline const juce::Colour kTextDim          { 0xFF808090 };  // Dimmed text
    static inline const juce::Colour kLcdBackground    { 0xFF0A0A14 };  // LCD display bg
    static inline const juce::Colour kLcdText          { 0xFF00FF88 };  // LCD green text
    static inline const juce::Colour kButtonOn         { 0xFF00CC66 };  // Active button
    static inline const juce::Colour kButtonOff        { 0xFF444455 };  // Inactive button

    // ---- Override drawing methods ----

    // Custom rotary knob drawing (the main control style)
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider& slider) override;

    // Custom linear slider drawing
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           juce::Slider::SliderStyle style, juce::Slider& slider) override;

    // Custom toggle button drawing (for voice on/off)
    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override;

    // Custom combo box drawing (for scale/key selection)
    void drawComboBox (juce::Graphics& g, int width, int height,
                       bool isButtonDown, int buttonX, int buttonY,
                       int buttonW, int buttonH, juce::ComboBox& box) override;

    // Custom label drawing
    void drawLabel (juce::Graphics& g, juce::Label& label) override;

    // ---- Utility drawing methods ----

    // Draw the brushed metal background texture
    static void drawBrushedMetal (juce::Graphics& g, juce::Rectangle<int> area);

    // Draw a recessed panel (for grouping controls)
    static void drawRecessedPanel (juce::Graphics& g, juce::Rectangle<int> area);

    // Draw an LCD-style display (for pitch readout)
    static void drawLcdDisplay (juce::Graphics& g, juce::Rectangle<int> area,
                                const juce::String& text, const juce::String& subText = "");
};
