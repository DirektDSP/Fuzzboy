/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PresetManager.h"
#include "Bitcrusher.h"

//==============================================================================
/**
*/
class FuzzboyAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    FuzzboyAudioProcessor();
    ~FuzzboyAudioProcessor() override;
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    float estimateFundamentalFrequency(const AudioBuffer<float>& buffer);

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    Service::PresetManager& getPresetManager() { return *presetManager; }

    AudioProcessorValueTreeState apvts;
    
	static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
	{
		std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
        
		params.push_back(std::make_unique<juce::AudioParameterFloat>("TONE", "Tone", 1.0f, 2.0f, 0.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>("POWER", "Power", 1.0f, 2.0f, 0.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>("IN", "In Gain", 0.0f, 5.0f, 1.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>("OUT", "Out Gain", 0.0f, 5.0f, 1.0f));
        
		params.push_back(std::make_unique<juce::AudioParameterBool>("BYPASS", "Bypass", false));

		params.push_back(std::make_unique<juce::AudioParameterChoice>("MODE", "Mode", juce::StringArray{"Crunch", "Potato", "Hiss"}, 0));
        
		return { params.begin(), params.end() };
	}

	AudioVisualiserComponent visualiser;
    
private:

    juce::AudioBuffer<float> lastProcessedBuffer;
    //==============================================================================
    //AudioProcessorValueTreeState apvts;
    std::unique_ptr<Service::PresetManager> presetManager;

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FuzzboyAudioProcessor)
};
