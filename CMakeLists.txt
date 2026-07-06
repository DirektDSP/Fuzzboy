cmake_minimum_required(VERSION 3.25)

# Fuzzboy — free DirektDSP fuzz/distortion plugin.
# Self-contained CMake build (JUCE fetched via CPM/FetchContent); no sibling
# PluginTemplate, no Moonbase licensing. Migrated from the legacy Projucer
# (.jucer) project to CMake + JUCE 8 by InstallerGenerator.

project(Fuzzboy VERSION 1.2.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- JUCE 8 via FetchContent (shallow, pinned) --------------------------------
include(FetchContent)
set(JUCE_VERSION "8.0.13" CACHE STRING "JUCE version tag to fetch")
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        ${JUCE_VERSION}
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(juce)

# --- CLAP support (free-audio extension) --------------------------------------
FetchContent_Declare(
    clap-juce-extensions
    GIT_REPOSITORY https://github.com/free-audio/clap-juce-extensions.git
    GIT_TAG        main
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(clap-juce-extensions)

# --- Plugin -------------------------------------------------------------------
juce_add_plugin(Fuzzboy
    COMPANY_NAME              "DirektDSP"
    COMPANY_WEBSITE           "https://direktdsp.com"
    BUNDLE_ID                 "com.direktdsp.fuzzboy"
    PLUGIN_MANUFACTURER_CODE  Dkdp          # was 'Manu' (Projucer default) — now the real DirektDSP code
    PLUGIN_CODE               Fb0a
    FORMATS                   VST3 AU CLAP Standalone
    VST3_CATEGORIES           Fx Distortion
    AU_MAIN_TYPE              kAudioUnitType_Effect
    COPY_PLUGIN_AFTER_BUILD   FALSE
    IS_SYNTH                  FALSE
    NEEDS_MIDI_INPUT          FALSE
    NEEDS_MIDI_OUTPUT         FALSE
    ICON_BIG                  "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
    PRODUCT_NAME              "Fuzzboy")

clap_juce_extensions_plugin(TARGET Fuzzboy
    CLAP_ID       "com.direktdsp.fuzzboy"
    CLAP_FEATURES audio-effect distortion)

# Source uses <JuceHeader.h>, `using namespace juce`, and ProjectInfo::versionString
# (both provided by the generated JuceHeader). Keep it — matches the legacy .jucer
# (addUsingNamespaceToJuceHeader="1") so the ported source compiles unchanged.
juce_generate_juce_header(Fuzzboy)

# --- Embedded brand assets (BinaryData) ---------------------------------------
# Only the images actually referenced from Source/ (StyleSheet.h + editor).
juce_add_binary_data(FuzzboyAssets SOURCES
    Assets/FuzzboyBG.png
    Assets/MiniBrute.png
    Assets/Toggle.png
    Assets/Toggle1.png
    Assets/swSlide0000.png
    Assets/swSlide0001.png)

target_sources(Fuzzboy PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/PresetManager.cpp)

target_compile_definitions(Fuzzboy PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_STRICT_REFCOUNTEDPOINTER=1)

target_link_libraries(Fuzzboy PRIVATE
    FuzzboyAssets
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_basics
    juce::juce_gui_extra
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
