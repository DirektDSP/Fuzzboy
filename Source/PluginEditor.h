#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "StyleSheet.h"
#include "PresetPanel.h"

#define PLUGIN_VERSION JucePlugin_VersionString

//==============================================================================
/**
*/
class FuzzboyAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FuzzboyAudioProcessorEditor (FuzzboyAudioProcessor&);
    ~FuzzboyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FuzzboyAudioProcessor& audioProcessor;

    Gui::PresetPanel presetPanel;

    ExtLabel versionLabel;

    RasterKnob toneSlider;
    RasterKnob powerSlider;
    BypassButton bypassButton;

    ModeCB modeBox;

    GainSlider inGain;
    GainSlider outGain;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> powerAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> scAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inGainAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outGainAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FuzzboyAudioProcessorEditor)
};
