#include "SkeuomorphicLookAndFeel.h"

// ============================================================================
// SkeuomorphicLookAndFeel Implementation
// ============================================================================

SkeuomorphicLookAndFeel::SkeuomorphicLookAndFeel()
{
    // Set default colors for the vintage theme
    setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    setColour (juce::Slider::textBoxTextColourId, kLcdText);
    setColour (juce::Slider::textBoxBackgroundColourId, kLcdBackground);
    setColour (juce::ComboBox::backgroundColourId, kPanelColor);
    setColour (juce::ComboBox::textColourId, kTextLight);
    setColour (juce::ComboBox::outlineColourId, kKnobHighlight);
    setColour (juce::ComboBox::arrowColourId, kAccentGold);
    setColour (juce::PopupMenu::backgroundColourId, kPanelColor);
    setColour (juce::PopupMenu::textColourId, kTextLight);
    setColour (juce::PopupMenu::highlightedBackgroundColourId, kAccentGold.withAlpha (0.3f));
    setColour (juce::PopupMenu::highlightedTextColourId, kTextLight);
    setColour (juce::Label::textColourId, kTextLight);
}

// ============================================================================
// Rotary Knob Drawing
// ============================================================================
// Creates a 3D-looking knob with a beveled edge and a pointer indicator.

void SkeuomorphicLookAndFeel::drawRotarySlider (juce::Graphics& g,
                                                 int x, int y, int width, int height,
                                                 float sliderPos,
                                                 float rotaryStartAngle,
                                                 float rotaryEndAngle,
                                                 juce::Slider& /*slider*/)
{
    // Calculate the knob dimensions
    float radius = (float) juce::jmin (width, height) / 2.0f - 4.0f;
    float centreX = (float) x + (float) width / 2.0f;
    float centreY = (float) y + (float) height / 2.0f;
    float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // ---- Outer ring (shadow) ----
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.fillEllipse (centreX - radius - 2, centreY - radius - 2,
                   radius * 2 + 4, radius * 2 + 4);

    // ---- Knob body ----
    // Create a gradient to simulate 3D lighting (light from top-left)
    juce::ColourGradient knobGradient (
        kKnobHighlight, centreX - radius * 0.5f, centreY - radius * 0.5f,
        kKnobBody.darker (0.3f), centreX + radius * 0.5f, centreY + radius * 0.5f,
        true);  // radial gradient

    g.setGradientFill (knobGradient);
    g.fillEllipse (centreX - radius, centreY - radius, radius * 2, radius * 2);

    // ---- Knob edge highlight ----
    g.setColour (kKnobHighlight.withAlpha (0.4f));
    g.drawEllipse (centreX - radius, centreY - radius, radius * 2, radius * 2, 1.5f);

    // ---- Pointer indicator ----
    // A line from the center pointing outward at the current angle
    juce::Path pointer;
    float pointerLength = radius * 0.7f;
    float pointerThickness = 3.0f;

    pointer.addRectangle (-pointerThickness / 2, -pointerLength, pointerThickness, pointerLength);

    // Rotate and translate the pointer to the correct position
    pointer.applyTransform (juce::AffineTransform::rotation (angle)
                                .translated (centreX, centreY));

    g.setColour (kAccentGold);
    g.fillPath (pointer);

    // ---- Center dot ----
    g.setColour (kKnobBody.darker (0.2f));
    g.fillEllipse (centreX - 4, centreY - 4, 8, 8);

    // ---- Position arc (shows the value visually) ----
    juce::Path arc;
    float arcRadius = radius + 6;
    arc.addCentredArc (centreX, centreY, arcRadius, arcRadius,
                       0.0f, rotaryStartAngle, angle, true);

    g.setColour (kAccentGold.withAlpha (0.7f));
    g.strokePath (arc, juce::PathStrokeType (2.5f));
}

// ============================================================================
// Linear Slider Drawing
// ============================================================================

void SkeuomorphicLookAndFeel::drawLinearSlider (juce::Graphics& g,
                                                 int x, int y, int width, int height,
                                                 float sliderPos, float /*minSliderPos*/,
                                                 float /*maxSliderPos*/,
                                                 juce::Slider::SliderStyle style,
                                                 juce::Slider& /*slider*/)
{
    bool isHorizontal = (style == juce::Slider::LinearHorizontal);

    // Track (the groove the slider moves in)
    juce::Rectangle<float> track;
    if (isHorizontal)
        track = { (float) x, (float) y + (float) height * 0.4f,
                  (float) width, (float) height * 0.2f };
    else
        track = { (float) x + (float) width * 0.4f, (float) y,
                  (float) width * 0.2f, (float) height };

    g.setColour (kLcdBackground);
    g.fillRoundedRectangle (track, 3.0f);

    // Filled portion (from start to current position)
    juce::Rectangle<float> filled;
    if (isHorizontal)
        filled = { (float) x, track.getY(), sliderPos - (float) x, track.getHeight() };
    else
        filled = { track.getX(), sliderPos, track.getWidth(), (float) (y + height) - sliderPos };

    g.setColour (kAccentGold);
    g.fillRoundedRectangle (filled, 3.0f);

    // Thumb (the draggable handle)
    float thumbSize = 14.0f;
    juce::Rectangle<float> thumb;
    if (isHorizontal)
        thumb = { sliderPos - thumbSize / 2, (float) y + (float) height * 0.5f - thumbSize / 2,
                  thumbSize, thumbSize };
    else
        thumb = { (float) x + (float) width * 0.5f - thumbSize / 2, sliderPos - thumbSize / 2,
                  thumbSize, thumbSize };

    g.setColour (kKnobHighlight);
    g.fillEllipse (thumb);
    g.setColour (kAccentGold);
    g.drawEllipse (thumb, 1.5f);
}

// ============================================================================
// Toggle Button Drawing (On/Off switches)
// ============================================================================

void SkeuomorphicLookAndFeel::drawToggleButton (juce::Graphics& g,
                                                 juce::ToggleButton& button,
                                                 bool /*shouldDrawButtonAsHighlighted*/,
                                                 bool /*shouldDrawButtonAsDown*/)
{
    bool isOn = button.getToggleState();
    auto bounds = button.getLocalBounds().toFloat().reduced (4);

    // Button background
    float cornerSize = 6.0f;
    g.setColour (isOn ? kButtonOn : kButtonOff);
    g.fillRoundedRectangle (bounds, cornerSize);

    // Button border
    g.setColour (isOn ? kButtonOn.brighter (0.3f) : kKnobHighlight);
    g.drawRoundedRectangle (bounds, cornerSize, 1.5f);

    // LED indicator (glowing dot when active)
    float ledSize = 8.0f;
    float ledX = bounds.getX() + 8;
    float ledY = bounds.getCentreY() - ledSize / 2;

    if (isOn)
    {
        // Glow effect
        g.setColour (kButtonOn.withAlpha (0.3f));
        g.fillEllipse (ledX - 3, ledY - 3, ledSize + 6, ledSize + 6);
    }
    g.setColour (isOn ? kButtonOn.brighter (0.5f) : kTextDim);
    g.fillEllipse (ledX, ledY, ledSize, ledSize);

    // Label text
    g.setColour (isOn ? kTextLight : kTextDim);
    g.setFont (13.0f);
    g.drawText (button.getButtonText(),
                bounds.withLeft (ledX + ledSize + 6).toNearestInt(),
                juce::Justification::centredLeft);
}

// ============================================================================
// Combo Box Drawing
// ============================================================================

void SkeuomorphicLookAndFeel::drawComboBox (juce::Graphics& g,
                                             int width, int height,
                                             bool /*isButtonDown*/,
                                             int /*buttonX*/, int /*buttonY*/,
                                             int /*buttonW*/, int /*buttonH*/,
                                             juce::ComboBox& /*box*/)
{
    auto bounds = juce::Rectangle<float> (0, 0, (float) width, (float) height);

    // Recessed background
    g.setColour (kLcdBackground);
    g.fillRoundedRectangle (bounds, 4.0f);

    // Border
    g.setColour (kKnobHighlight.withAlpha (0.5f));
    g.drawRoundedRectangle (bounds.reduced (0.5f), 4.0f, 1.0f);

    // Down arrow
    juce::Path arrow;
    float arrowX = (float) width - 20.0f;
    float arrowY = (float) height * 0.5f - 3.0f;
    arrow.addTriangle (arrowX, arrowY, arrowX + 10, arrowY, arrowX + 5, arrowY + 6);

    g.setColour (kAccentGold);
    g.fillPath (arrow);
}

// ============================================================================
// Label Drawing
// ============================================================================

void SkeuomorphicLookAndFeel::drawLabel (juce::Graphics& g, juce::Label& label)
{
    g.setColour (label.findColour (juce::Label::textColourId));
    g.setFont (label.getFont());

    auto textArea = label.getBorderSize().subtractedFrom (label.getLocalBounds());
    g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
                      juce::jmax (1, (int) ((float) textArea.getHeight() / label.getFont().getHeight())),
                      label.getMinimumHorizontalScale());
}

// ============================================================================
// Utility Drawing Methods
// ============================================================================

void SkeuomorphicLookAndFeel::drawBrushedMetal (juce::Graphics& g,
                                                 juce::Rectangle<int> area)
{
    // Create a brushed metal effect using overlapping gradients
    juce::ColourGradient mainGradient (
        kBackgroundDark, 0, (float) area.getY(),
        kBackgroundMid, 0, (float) area.getBottom(),
        false);

    g.setGradientFill (mainGradient);
    g.fillRect (area);

    // Add subtle horizontal "brush strokes" using semi-transparent lines
    juce::Random rng (42);  // Fixed seed for consistent appearance
    g.setColour (juce::Colours::white.withAlpha (0.02f));

    for (int i = area.getY(); i < area.getBottom(); i += 2)
    {
        if (rng.nextFloat() > 0.5f)
            g.drawHorizontalLine (i, (float) area.getX(), (float) area.getRight());
    }
}

void SkeuomorphicLookAndFeel::drawRecessedPanel (juce::Graphics& g,
                                                  juce::Rectangle<int> area)
{
    // Dark recessed background
    g.setColour (kPanelColor.darker (0.3f));
    g.fillRoundedRectangle (area.toFloat(), 6.0f);

    // Inner shadow (top and left edges are darker)
    g.setColour (juce::Colours::black.withAlpha (0.3f));
    g.drawRoundedRectangle (area.toFloat().reduced (0.5f), 6.0f, 1.0f);

    // Slight highlight on bottom/right edges
    g.setColour (juce::Colours::white.withAlpha (0.05f));
    g.drawLine ((float) area.getX() + 6, (float) area.getBottom() - 1,
                (float) area.getRight() - 6, (float) area.getBottom() - 1);
}

void SkeuomorphicLookAndFeel::drawLcdDisplay (juce::Graphics& g,
                                               juce::Rectangle<int> area,
                                               const juce::String& text,
                                               const juce::String& subText)
{
    // LCD background
    g.setColour (kLcdBackground);
    g.fillRoundedRectangle (area.toFloat(), 4.0f);

    // LCD border (slight bevel)
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.drawRoundedRectangle (area.toFloat(), 4.0f, 1.5f);

    // Main text (large, green, LCD-style)
    g.setColour (kLcdText);
    g.setFont (juce::Font ((float) area.getHeight() * 0.45f));

    if (subText.isEmpty())
    {
        g.drawText (text, area, juce::Justification::centred);
    }
    else
    {
        auto topHalf = area.removeFromTop (area.getHeight() / 2);
        g.drawText (text, topHalf, juce::Justification::centredBottom);

        g.setColour (kLcdText.withAlpha (0.6f));
        g.setFont (juce::Font ((float) area.getHeight() * 0.6f));
        g.drawText (subText, area, juce::Justification::centredTop);
    }
}
