#include "PluginProcessor.h"
#include "PluginEditor.h"

// ============================================================================
// PluginProcessor Implementation
// ============================================================================

HoggleTuneProcessor::HoggleTuneProcessor()
    : AudioProcessor (BusesProperties()
                        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "Parameters", createParameterLayout())
{
}

HoggleTuneProcessor::~HoggleTuneProcessor()
{
}

// ============================================================================
// Parameter Layout
// ============================================================================
// This defines every knob, slider, and switch in the plugin.
// Each parameter has an ID, name, range, and default value.

juce::AudioProcessorValueTreeState::ParameterLayout HoggleTuneProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // ---- Pitch Correction ----
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_CORRECTION_SPEED, 1 },
        "Correction Speed",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f));  // Default: 50% — moderate correction

    // Root note selector (0=C, 1=C#, 2=D, ... 11=B)
    params.push_back (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID { PARAM_ROOT_NOTE, 1 },
        "Root Note",
        0, 11, 0));  // Default: C

    // Scale mode selector (0=Chromatic, 1=Ionian, ... 7=Locrian)
    params.push_back (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID { PARAM_SCALE_MODE, 1 },
        "Scale Mode",
        0, 7, 0));  // Default: Chromatic

    // ---- Input/Output ----
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_INPUT_GAIN, 1 },
        "Input Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
        0.0f));  // Default: 0 dB (unity gain)

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_OUTPUT_GAIN, 1 },
        "Output Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
        0.0f));  // Default: 0 dB

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_DRY_WET_MIX, 1 },
        "Dry/Wet Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        1.0f));  // Default: 100% wet (fully processed)

    // ---- Harmonizer Voices ----
    // Voice 1
    params.push_back (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { PARAM_VOICE1_ENABLED, 1 }, "Voice 1 On", false));
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_VOICE1_INTERVAL, 1 }, "Voice 1 Interval",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 1.0f), 4.0f));  // Major third
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_VOICE1_GAIN, 1 }, "Voice 1 Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.5f));

    // Voice 2
    params.push_back (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { PARAM_VOICE2_ENABLED, 1 }, "Voice 2 On", false));
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_VOICE2_INTERVAL, 1 }, "Voice 2 Interval",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 1.0f), 7.0f));  // Perfect fifth
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_VOICE2_GAIN, 1 }, "Voice 2 Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.5f));

    // Voice 3
    params.push_back (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { PARAM_VOICE3_ENABLED, 1 }, "Voice 3 On", false));
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_VOICE3_INTERVAL, 1 }, "Voice 3 Interval",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 1.0f), 12.0f));  // Octave
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_VOICE3_GAIN, 1 }, "Voice 3 Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.3f));

    // Voice 4
    params.push_back (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { PARAM_VOICE4_ENABLED, 1 }, "Voice 4 On", false));
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_VOICE4_INTERVAL, 1 }, "Voice 4 Interval",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 1.0f), -12.0f));  // Octave down
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_VOICE4_GAIN, 1 }, "Voice 4 Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.3f));

    // ---- Effects ----
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_DISTORTION, 1 },
        "Distortion",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.08f));  // Default: 8% (under 10%)

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_REVERB_AMOUNT, 1 },
        "Reverb Amount",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.08f));  // Default: 8% (under 10%)

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_REVERB_SIZE, 1 },
        "Reverb Size",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f));  // Default: medium room

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_DELAY_TIME, 1 },
        "Delay Time",
        juce::NormalisableRange<float> (20.0f, 2000.0f, 1.0f),
        300.0f));  // Default: 300ms

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_DELAY_FEEDBACK, 1 },
        "Delay Feedback",
        juce::NormalisableRange<float> (0.0f, 0.9f, 0.01f),
        0.3f));  // Default: 30%

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { PARAM_DELAY_MIX, 1 },
        "Delay Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.05f));  // Default: 5% (under 6%)

    return { params.begin(), params.end() };
}

// ============================================================================
// Prepare to Play
// ============================================================================
// Called by the DAW before audio processing starts. This is where we
// allocate buffers and initialize all DSP components.

void HoggleTuneProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize all DSP components with the current sample rate
    autotuneEngine.prepare (sampleRate, samplesPerBlock);
    harmonizerEngine.prepare (sampleRate, samplesPerBlock);
    distortionProcessor.prepare (sampleRate);
    reverbProcessor.prepare (sampleRate, samplesPerBlock);
    delayProcessor.prepare (sampleRate, samplesPerBlock);

    // Pre-allocate working buffers
    monoBuffer.resize ((size_t) samplesPerBlock, 0.0f);
    harmonyBuffer.resize ((size_t) samplesPerBlock, 0.0f);
}

void HoggleTuneProcessor::releaseResources()
{
    // Nothing to release — JUCE handles cleanup
}

// ============================================================================
// Process Block — The Main Audio Callback
// ============================================================================
// This is called ~44100/bufferSize times per second (e.g., ~86 times/sec
// at 44100 Hz with 512-sample buffers). It MUST be real-time safe:
// no allocation, no locks, no exceptions.

void HoggleTuneProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                         juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;  // Prevent CPU spikes from denormalized floats

    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    // Read all parameters and apply them to DSP components
    updateParameters();

    // ---- Step 1: Apply input gain ----
    float inputGainDb = apvts.getRawParameterValue (PARAM_INPUT_GAIN)->load();
    float inputGain = juce::Decibels::decibelsToGain (inputGainDb);

    for (int ch = 0; ch < numChannels; ++ch)
        buffer.applyGain (ch, 0, numSamples, inputGain);

    // ---- Step 2: Create mono mix for pitch processing ----
    // Sum all channels to mono (most pitch processing works in mono)
    for (int i = 0; i < numSamples; ++i)
    {
        float sum = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
            sum += buffer.getSample (ch, i);
        monoBuffer[(size_t) i] = sum / (float) numChannels;
    }

    // Save dry signal for dry/wet mixing
    float dryWetMix = apvts.getRawParameterValue (PARAM_DRY_WET_MIX)->load();

    // ---- Step 3: Autotune (pitch correction) ----
    autotuneEngine.process (monoBuffer.data(), numSamples);

    // ---- Step 4: Harmonizer ----
    // Clear harmony buffer and fill it with harmony voices
    std::fill (harmonyBuffer.begin(), harmonyBuffer.begin() + numSamples, 0.0f);
    harmonizerEngine.process (monoBuffer.data(), harmonyBuffer.data(), numSamples);

    // Mix harmonizer output into the mono buffer
    for (int i = 0; i < numSamples; ++i)
        monoBuffer[(size_t) i] += harmonyBuffer[(size_t) i];

    // ---- Step 5: Effects chain ----
    distortionProcessor.process (monoBuffer.data(), numSamples);
    reverbProcessor.process (monoBuffer.data(), numSamples);
    delayProcessor.process (monoBuffer.data(), numSamples);

    // ---- Step 6: Apply dry/wet mix and copy to all output channels ----
    float outputGainDb = apvts.getRawParameterValue (PARAM_OUTPUT_GAIN)->load();
    float outputGain = juce::Decibels::decibelsToGain (outputGainDb);

    for (int ch = 0; ch < numChannels; ++ch)
    {
        float* channelData = buffer.getWritePointer (ch);
        for (int i = 0; i < numSamples; ++i)
        {
            // Blend between dry (original) and wet (processed) signal
            float dry = channelData[i];
            float wet = monoBuffer[(size_t) i];
            channelData[i] = (dry * (1.0f - dryWetMix) + wet * dryWetMix) * outputGain;
        }
    }
}

// ============================================================================
// Update Parameters
// ============================================================================
// Reads all parameter values from APVTS and pushes them to DSP components.
// Called once per processBlock.

void HoggleTuneProcessor::updateParameters()
{
    // Pitch correction parameters
    autotuneEngine.setCorrectionSpeed (apvts.getRawParameterValue (PARAM_CORRECTION_SPEED)->load());
    autotuneEngine.setRootNote (static_cast<ScaleEngine::RootNote> (
        apvts.getRawParameterValue (PARAM_ROOT_NOTE)->load()));
    autotuneEngine.setMode (static_cast<ScaleEngine::Mode> (
        apvts.getRawParameterValue (PARAM_SCALE_MODE)->load()));

    // Harmonizer voices
    harmonizerEngine.setVoiceEnabled (0, apvts.getRawParameterValue (PARAM_VOICE1_ENABLED)->load() > 0.5f);
    harmonizerEngine.setVoiceInterval (0, apvts.getRawParameterValue (PARAM_VOICE1_INTERVAL)->load());
    harmonizerEngine.setVoiceGain (0, apvts.getRawParameterValue (PARAM_VOICE1_GAIN)->load());

    harmonizerEngine.setVoiceEnabled (1, apvts.getRawParameterValue (PARAM_VOICE2_ENABLED)->load() > 0.5f);
    harmonizerEngine.setVoiceInterval (1, apvts.getRawParameterValue (PARAM_VOICE2_INTERVAL)->load());
    harmonizerEngine.setVoiceGain (1, apvts.getRawParameterValue (PARAM_VOICE2_GAIN)->load());

    harmonizerEngine.setVoiceEnabled (2, apvts.getRawParameterValue (PARAM_VOICE3_ENABLED)->load() > 0.5f);
    harmonizerEngine.setVoiceInterval (2, apvts.getRawParameterValue (PARAM_VOICE3_INTERVAL)->load());
    harmonizerEngine.setVoiceGain (2, apvts.getRawParameterValue (PARAM_VOICE3_GAIN)->load());

    harmonizerEngine.setVoiceEnabled (3, apvts.getRawParameterValue (PARAM_VOICE4_ENABLED)->load() > 0.5f);
    harmonizerEngine.setVoiceInterval (3, apvts.getRawParameterValue (PARAM_VOICE4_INTERVAL)->load());
    harmonizerEngine.setVoiceGain (3, apvts.getRawParameterValue (PARAM_VOICE4_GAIN)->load());

    // Effects
    distortionProcessor.setAmount (apvts.getRawParameterValue (PARAM_DISTORTION)->load());
    reverbProcessor.setAmount (apvts.getRawParameterValue (PARAM_REVERB_AMOUNT)->load());
    reverbProcessor.setRoomSize (apvts.getRawParameterValue (PARAM_REVERB_SIZE)->load());
    delayProcessor.setDelayTimeMs (apvts.getRawParameterValue (PARAM_DELAY_TIME)->load());
    delayProcessor.setFeedback (apvts.getRawParameterValue (PARAM_DELAY_FEEDBACK)->load());
    delayProcessor.setWetMix (apvts.getRawParameterValue (PARAM_DELAY_MIX)->load());
}

// ============================================================================
// State Save/Load — So DAW sessions remember plugin settings
// ============================================================================

void HoggleTuneProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Serialize all parameters to XML
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void HoggleTuneProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Deserialize parameters from XML
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

// ============================================================================
// Editor (GUI) Creation
// ============================================================================

juce::AudioProcessorEditor* HoggleTuneProcessor::createEditor()
{
    return new HoggleTuneEditor (*this);
}

// ============================================================================
// Plugin Instantiation
// ============================================================================
// This function is called by the DAW to create a new instance of our plugin.
// It's the entry point — like main() for a plugin.

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HoggleTuneProcessor();
}
