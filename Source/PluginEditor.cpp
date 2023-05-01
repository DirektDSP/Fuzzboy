/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define PLUGIN_VERSION JucePlugin_VersionString

int textBoxWidth = 40;
int textBoxHeight = 20;

//==============================================================================
FuzzboyAudioProcessorEditor::FuzzboyAudioProcessorEditor (FuzzboyAudioProcessor& p) :
		AudioProcessorEditor (&p),
		audioProcessor (p),
		presetPanel(p.getPresetManager())
{

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	
	addAndMakeVisible(presetPanel);
	
	
    setSize (400, 650);
	
	toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TONE", toneSlider);
	powerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "POWER", powerSlider);
	bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "BYPASS", bypassButton);
	inGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "IN", inGain);
	outGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OUT", outGain);
	modeBox.addItemList(juce::StringArray{ "Crunch", "Potato", "Hiss" }, 1);
	modeBox.setSelectedItemIndex(0);
	modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "MODE", modeBox);
	
}

FuzzboyAudioProcessorEditor::~FuzzboyAudioProcessorEditor()
{
}

//==============================================================================
void FuzzboyAudioProcessorEditor::paint (juce::Graphics& g)
{
	auto backgroundImage = juce::ImageCache::getFromMemory(BinaryData::FuzzboyBG_png, BinaryData::FuzzboyBG_pngSize);
	
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

	// draw bg image
	g.drawImage(backgroundImage, getLocalBounds().toFloat());
	

	toneSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	toneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
	toneSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
	toneSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(48, 48, 48));
	toneSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
	addAndMakeVisible(toneSlider);

	powerSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	powerSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
	powerSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
	powerSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(48, 48, 48));
	powerSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
	addAndMakeVisible(powerSlider);

	inGain.setSliderStyle(juce::Slider::LinearVertical);
	inGain.setTextBoxStyle(juce::Slider::TextBoxAbove, false, textBoxWidth + 15, textBoxHeight);
	inGain.setTextValueSuffix(" dB");
	inGain.setNumDecimalPlacesToDisplay(2);
	inGain.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
	inGain.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(48, 48, 48));
	inGain.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
	addAndMakeVisible(inGain);

	outGain.setSliderStyle(juce::Slider::LinearVertical);
	outGain.setTextBoxStyle(juce::Slider::TextBoxAbove, false, textBoxWidth + 15, textBoxHeight);
	outGain.setTextValueSuffix(" dB");
	outGain.setNumDecimalPlacesToDisplay(2);
	outGain.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
	outGain.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(48, 48, 48));
	outGain.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
	addAndMakeVisible(outGain);

	bypassButton.setButtonText("Bypass");
	bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
	bypassButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
	addAndMakeVisible(bypassButton);
	
	addAndMakeVisible(modeBox);

	versionLabel.setText("Version " PLUGIN_VERSION, juce::dontSendNotification);
	versionLabel.setColour(juce::Label::textColourId, juce::Colours::black);
	addAndMakeVisible(versionLabel);
	

	addAndMakeVisible(audioProcessor.visualiser);
	audioProcessor.visualiser.setColours(juce::Colour::fromRGB(204, 204, 204), Colour::fromRGB(42, 42, 42));
}

void FuzzboyAudioProcessorEditor::resized()
{
	presetPanel.setBounds(getLocalBounds().removeFromTop(proportionOfHeight(0.1f)));
	
	//window is 400 x 600
	/*
	int border = 20; // between knobs
	int padding = 50; // knob and edge
	
	int ppOffset = presetPanel.getHeight() + padding + 45;
	
	auto row1 = Rectangle<int>(padding, ppOffset, 300, 110);

	Grid g1;
	using Track = Grid::TrackInfo;
	g1.templateRows = { Track(1_fr) };
	g1.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr) };
	g1.columnGap = Grid::Px(border);
	g1.items = { 
		GridItem(gainSlider),
		GridItem(freqSlider),
		GridItem(clipSlider),
	};
	g1.performLayout(row1);

	auto row2 = Rectangle<int>(padding, ppOffset + 110 + border, 300, 110);
	
	Grid g2;
	g2.templateRows = { Track(1_fr) };
	g2.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr) };
	g2.columnGap = Grid::Px(border);
	g2.items = {
		GridItem(powerSlider),
		GridItem(inGain),
		GridItem(outGain),
	};
	g2.performLayout(row2);
	*/


	// x=190/500 y=900
	// bypass button
	int bypSize = 75;
	int knobSize = 118/2;
	auto y = getLocalBounds().removeFromTop(proportionOfHeight(0.1f)).getBottomLeft().getY();
	int toggleHeight = 450;

	// account for textbox
	toneSlider.setBounds(190/2, 385, knobSize, knobSize + textBoxHeight);
	powerSlider.setBounds(492/2, 385, knobSize, knobSize + textBoxHeight);
	
	
	modeBox.setBounds(getWidth()/6, y + 10, getWidth()/1.5, 25);

	inGain.setBounds(5, 2*y + 15, 60, 435);
	outGain.setBounds(getWidth() - 65, 2 * y + 15, 60, 435);
	

	
	bypassButton.setBounds(getWidth() / 2 - (bypSize / 2), toggleHeight, bypSize, bypSize);
	

	versionLabel.setBounds(getBounds().removeFromBottom(20).removeFromLeft(100));


	// put under the dropdown box
	audioProcessor.visualiser.setBounds(getWidth() / 6, y + 40, getWidth() / 1.5, 25);
	
}
