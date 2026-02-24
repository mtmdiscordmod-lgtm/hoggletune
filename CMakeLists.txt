cmake_minimum_required(VERSION 3.22)

# ============================================================================
# HoggleTune — Autotune/Harmonizer Audio Plugin
# ============================================================================
# This is the main build file. It downloads JUCE automatically and sets up
# the plugin for AU, VST3, and AAX formats.
#
# To build:
#   mkdir build && cd build
#   cmake ..
#   cmake --build .
# ============================================================================

project(HoggleTune VERSION 1.0.0 LANGUAGES CXX)

# We need C++17 for modern features used throughout the codebase
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download JUCE automatically using CMake's FetchContent
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        7.0.12
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(JUCE)

# ============================================================================
# Plugin Target
# ============================================================================
# juce_add_plugin sets up the plugin with all the metadata DAWs need.
# It creates targets for each format (AU, VST3, AAX).
juce_add_plugin(HoggleTune
    COMPANY_NAME                "HoggleTune"
    IS_SYNTH                    FALSE           # This is an effect, not a synth
    NEEDS_MIDI_INPUT            FALSE           # We don't process MIDI
    NEEDS_MIDI_OUTPUT           FALSE
    IS_MIDI_EFFECT              FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD     TRUE            # Auto-install after building
    PLUGIN_MANUFACTURER_CODE    Hglt            # 4-char manufacturer code
    PLUGIN_CODE                 HgTn            # 4-char plugin code
    FORMATS                     AU VST3 Standalone  # Plugin formats to build
    PRODUCT_NAME                "HoggleTune"
    BUNDLE_ID                   "com.hoggletune.hoggletune"
    MICROPHONE_PERMISSION_ENABLED TRUE          # Standalone needs mic access
    MICROPHONE_PERMISSION_TEXT  "HoggleTune needs microphone access for real-time pitch correction."
)

# ============================================================================
# Source Files
# ============================================================================
# List every source file the plugin needs. CMake compiles all of these.
target_sources(HoggleTune
    PRIVATE
        # Main plugin files
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp

        # DSP (Digital Signal Processing) components
        Source/DSP/PitchDetector.cpp
        Source/DSP/PitchShifter.cpp
        Source/DSP/ScaleEngine.cpp
        Source/DSP/AutotuneEngine.cpp
        Source/DSP/HarmonizerEngine.cpp
        Source/DSP/DistortionProcessor.cpp
        Source/DSP/ReverbProcessor.cpp
        Source/DSP/DelayProcessor.cpp

        # GUI components
        Source/GUI/SkeuomorphicLookAndFeel.cpp
)

# ============================================================================
# JUCE Module Dependencies
# ============================================================================
# These are the JUCE modules our plugin uses. Each module provides
# different functionality (audio processing, GUI, DSP utilities, etc.)
target_link_libraries(HoggleTune
    PRIVATE
        juce::juce_audio_basics          # Core audio types (AudioBuffer, etc.)
        juce::juce_audio_devices         # Audio hardware I/O
        juce::juce_audio_formats         # Audio file reading/writing
        juce::juce_audio_plugin_client   # Plugin format wrappers (AU/VST3/AAX)
        juce::juce_audio_processors      # AudioProcessor base class
        juce::juce_audio_utils           # Audio utility classes
        juce::juce_core                  # Core utilities (String, File, etc.)
        juce::juce_dsp                   # DSP utilities (Reverb, filters, etc.)
        juce::juce_graphics              # 2D graphics (for our GUI)
        juce::juce_gui_basics            # GUI components (sliders, buttons, etc.)
        juce::juce_gui_extra             # Extra GUI utilities
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# ============================================================================
# Compile Definitions
# ============================================================================
# These preprocessor defines configure JUCE at compile time.
target_compile_definitions(HoggleTune
    PUBLIC
        JUCE_WEB_BROWSER=0               # We don't need a web browser
        JUCE_USE_CURL=0                   # We don't need HTTP
        JUCE_VST3_CAN_REPLACE_VST2=0     # No VST2 replacement
        JUCE_DISPLAY_SPLASH_SCREEN=0      # No JUCE splash screen
)

# Include directories so #include "DSP/PitchDetector.h" works
target_include_directories(HoggleTune
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)
