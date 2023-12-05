/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "StyleSheet.h"
#include <random>

//==============================================================================
FuzzboyAudioProcessor::FuzzboyAudioProcessor()

#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                    ),
#endif
            apvts(*this, nullptr, "Parameters", createParameterLayout()),
    visualiser(1)
{
    apvts.state.setProperty(Service::PresetManager::presetNameProperty, "", nullptr);
    apvts.state.setProperty("version", ProjectInfo::versionString, nullptr);

    presetManager = std::make_unique<Service::PresetManager>(apvts);
    
    visualiser.setRepaintRate(45);
    visualiser.setBufferSize(512);
    
};

FuzzboyAudioProcessor::~FuzzboyAudioProcessor()
{
    
}



//==============================================================================
const juce::String FuzzboyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FuzzboyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FuzzboyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FuzzboyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FuzzboyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FuzzboyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FuzzboyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FuzzboyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FuzzboyAudioProcessor::getProgramName (int index)
{
    return {};
}

void FuzzboyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FuzzboyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    
}

void FuzzboyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FuzzboyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

/*
float distort(float input, float gain, float freq, float clipAmt, float power) {
    float output = tanh(gain * input);
    float pi = 3.14159265358979323846;
    output = (sin(pi * output * freq) + sinh(pi * input )) / 2;
    output = tanh(output * power);
    output = output * 0.6f;
    
    return juce::jlimit<float>(-clipAmt, clipAmt, output);
}
*/

float softClip(float x)
{
    return tanh(x);
}

float foldback(float in, float threshold)
{
    if (in > threshold || in < -threshold)
    {
        in = std::abs(std::abs(std::fmod(in - threshold, threshold * 4)) - threshold * 2) - threshold;
    }
    return in;
}

float HPdist(float x, float g, float f, float p)
{
    float pi = 3.14159265358979323846;
    float output = tanh(g * x);
    output = (sin(pi * output * f) + tanh(pi * x)) / 2;
    output = (tanh((output / pi) * p) + tanh((x * pi) / (pi * pi))) * (4 / (p + 1)) * (1 + (p / 10));
    return output * 0.8f;
}

double euler = 2.71828182845904523536;

float sour(float t, float p, float x) {
    double tx2 = t * x * x; // calculate t * x^2
    double sign_x = (x < 0) ? -1 : 1; // calculate sign of -x
    double exp_part = (1 - exp(abs(x))) / (euler - 1); // calculate the exponential part
    return tx2 * sign_x * exp_part; // combine the parts
}


void FuzzboyAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.


    // get parameter values
    auto t = apvts.getRawParameterValue("TONE")->load();
    auto p = apvts.getRawParameterValue("POWER")->load();
    bool bp = apvts.getRawParameterValue("BYPASS")->load();
    
    int mode = apvts.getRawParameterValue("MODE")->load();

    float in = apvts.getRawParameterValue("IN")->load();
    float out = apvts.getRawParameterValue("OUT")->load();

    
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
            // clip the signal to the amp value
            for (int channel = 0; channel < totalNumInputChannels; ++channel)
            {
                auto* channelData = buffer.getWritePointer(channel);
                float x = channelData[sample];
                
                
                
                if (bp == false)
                {
                    x *= in;
                    switch(mode){
                        case 0:{
                            // crunch
                            x = 2 * x / (1 + sin(2 * x));
                            x *= t * p * p;
                            x = softClip(x);
                            x *= out;
                            
                            channelData[sample] = x;
                            break;
                        }
                        case 1:{
                            // smooth
                            x = HPdist(x, t, p, 10);
                            x *= t;
                            x = softClip(x);
                            x *= out;
                            
                            channelData[sample] = x;
                            break;
                        }
                        case 2:{
                            // hiss
                            float r3 = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f + 1.0f)));
                            r3 *= x;
                            r3 *= 0.001 * (p-1);
                            x += r3;
                            
                            x = p * tan(sin(x));
                            x = ((sinh(p * x)) * (2 * p)) / (2);
                            
                            x = softClip(3 * t * x);
                            channelData[sample] = x;
                            break;
                        }
                        case 3:{
                            x = sour(t, p, x);
                            x *= out; // Apply output gain
                            channelData[sample] = x;
                            break;
                        }
                        default:{
                            x *= out;
                            break;
                        }
//                        channelData[sample] = x;
                    }
                    
                }
                else
                {
                    channelData[sample] = x;
                }

            }
    }
    visualiser.pushBuffer(buffer);
};



//==============================================================================
bool FuzzboyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FuzzboyAudioProcessor::createEditor()
{
    return new FuzzboyAudioProcessorEditor (*this);
}

//==============================================================================
void FuzzboyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FuzzboyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FuzzboyAudioProcessor();
}
