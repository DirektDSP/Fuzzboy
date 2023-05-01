#pragma once

#include <JuceHeader.h>
#include "PluginEditor.h"
#include "PluginProcessor.h"

class RasterKnob : public juce::Slider
{
public:

	RasterKnob() : juce::Slider(SliderStyle::RotaryHorizontalVerticalDrag, TextEntryBoxPosition::NoTextBox)
	{	
		setMouseCursor(MouseCursor::PointingHandCursor);
		setLookAndFeel(&mainSliderLookAndFeel);
	}

	~RasterKnob()
	{
		setLookAndFeel(nullptr);
	}

	class RasterKnobLookAndFeel : public juce::LookAndFeel_V4
	{
	public:

		RasterKnobLookAndFeel()
		{
			image = juce::ImageCache::getFromMemory(BinaryData::MiniBrute_png, BinaryData::MiniBrute_pngSize);
			// byImage = juce::ImageCache::getFromMemory(BinaryData::LittlePhatty_OFF_png, BinaryData::LittlePhatty_OFF_pngSize);
		}

		~RasterKnobLookAndFeel() override
		{
		}
		
		void drawRotarySlider(
			juce::Graphics& g,
			int x,
			int y,
			int width,
			int height,
			float sliderPosProportional,
			float /*rotaryStartAngle*/,
			float /*rotaryEndAngle*/,
			juce::Slider& /*slider*/) override
		{
			const auto frames = 101;
			const auto frameId = static_cast<int>(ceil(sliderPosProportional * (static_cast<float>(frames) - 1.0f)));

			// image is vertical strip, each frame is 1/100th of the height

			g.drawImage(image,
				x,
				y,
				width,
				height,
				0,
				frameId * image.getHeight() / frames,
				image.getWidth(),
				image.getHeight() / frames);
		}

	private:
		juce::Image image;
		juce::Image byImage;

		bool bp = false;


		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RasterKnobLookAndFeel)
	} mainSliderLookAndFeel;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RasterKnob)
};


//
//class BypassButton : public juce::ToggleButton
//{
//
//public:
//	BypassButton() : juce::ToggleButton("")
//	{
//		setLookAndFeel(&mainButtonLookAndFeel);
//	}
//
//	~BypassButton()
//	{
//		setLookAndFeel(nullptr);
//	}
//
//private:
//	class BypassButtonLookAndFeel : public juce::LookAndFeel_V4
//	{
//	public:
//		BypassButtonLookAndFeel()
//		{
//			image = ImageCache::getFromMemory(BinaryData::Toggle_png, BinaryData::Toggle_pngSize);
//		}
//
//
//		void drawTickBox(Graphics& g, Component& component,
//            float x,
//            float y,
//            float w,
//            float h,
//            bool ticked,
//            bool isEnabled,
//            bool isMouseOverButton,
//            bool isButtonDown) override
//		{
//			const auto frames = 2;
//			const auto frameId = static_cast<int>(ticked);
//
//            Rectangle<float> Bounds(x, y, w, h);
//
//			g.drawImage(image,
//				x,
//				y,
//				w,
//				h,
//				0,
//				frameId * image.getHeight() / frames,
//				image.getWidth(),
//				image.getHeight() / frames);
//		}
//        
//
//	private:
//		juce::Image image;
//            
//		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BypassButtonLookAndFeel)
//	} mainButtonLookAndFeel;
//
//	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BypassButton)
//};

class BypassButton : public juce::ToggleButton
{

public:
	BypassButton() : juce::ToggleButton("")
	{
		setLookAndFeel(&mainButtonLookAndFeel);
		setImage();
	}

	~BypassButton()
	{
		setLookAndFeel(nullptr);
	}

	void setImage()
	{
		offImage_ = ImageCache::getFromMemory(BinaryData::Toggle_png, BinaryData::Toggle_pngSize);
		onImage_ = ImageCache::getFromMemory(BinaryData::Toggle1_png, BinaryData::Toggle1_pngSize);
	}

	void paintButton(Graphics& g, bool isMouseOver, bool isButtonDown) override
	{
		const auto frameId = static_cast<int>(getToggleState());
		const auto image = frameId ? onImage_ : offImage_;
		g.drawImage(image, getLocalBounds().toFloat());


	}

private:
	juce::Image offImage_;
	juce::Image onImage_;



	class BypassButtonLookAndFeel : public juce::LookAndFeel_V4
	{
	public:
		BypassButtonLookAndFeel()
		{
		}

		void drawTickBox(Graphics& g, Component& component,
			float x,
			float y,
			float w,
			float h,
			bool ticked,
			bool isEnabled,
			bool isMouseOverButton,
			bool isButtonDown) override
		{
			// Do nothing, we're not drawing a tick box
		}

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BypassButtonLookAndFeel)
	} mainButtonLookAndFeel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BypassButton)
};

class ToggleSwitchButton : public juce::ToggleButton
{

public:
	ToggleSwitchButton() : juce::ToggleButton("")
	{
		setLookAndFeel(&mainButtonLookAndFeel);
		setImage();
	}

	~ToggleSwitchButton()
	{
		setLookAndFeel(nullptr);
	}

	void setImage()
	{
		offImage_ = ImageCache::getFromMemory(BinaryData::swSlide0000_png, BinaryData::swSlide0000_pngSize);
		onImage_ = ImageCache::getFromMemory(BinaryData::swSlide0001_png, BinaryData::swSlide0001_pngSize);
	}

	void paintButton(Graphics& g, bool isMouseOver, bool isButtonDown) override
	{
		const auto frameId = static_cast<int>(getToggleState());
		const auto image = frameId ? onImage_ : offImage_;
		g.drawImage(image, getLocalBounds().toFloat());


	}

private:
	juce::Image offImage_;
	juce::Image onImage_;



	class ToggleSwitchButtonLookAndFeel : public juce::LookAndFeel_V4
	{
	public:
		ToggleSwitchButtonLookAndFeel()
		{
		}

		void drawTickBox(Graphics& g, Component& component,
			float x,
			float y,
			float w,
			float h,
			bool ticked,
			bool isEnabled,
			bool isMouseOverButton,
			bool isButtonDown) override
		{
			// Do nothing, we're not drawing a tick box
		}

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToggleSwitchButtonLookAndFeel)
	} mainButtonLookAndFeel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToggleSwitchButton)
};

class GainSlider : public juce::Slider
{
public:
	GainSlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
	{
		setLookAndFeel(&mainSliderLookAndFeel);
		setRange(-60.0f, 12.0f, 0.1f);
		setValue(0.0f);
		setTextValueSuffix(" dB");
		setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colour::fromRGB(42, 42, 42));
		setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colour::fromRGB(42, 42, 42));
		setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::white);
		setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::white);
	}

	~GainSlider()
	{
		setLookAndFeel(nullptr);
	}
	
private:

	class GainSliderLookAndFeel : public juce::LookAndFeel_V4
	{
	public:
		GainSliderLookAndFeel()
		{
			//setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::white);
			//setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::white);
			//setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::white);
			//setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::white);
		}

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainSliderLookAndFeel)
	} mainSliderLookAndFeel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainSlider)
};

class PresetButton : public juce::TextButton
{
public:
	PresetButton() : juce::TextButton("")
	{
		setLookAndFeel(&mainButtonLookAndFeel);
	}

	~PresetButton()
	{
		setLookAndFeel(nullptr);
	}

private:
	class PresetButtonLookAndFeel : public juce::LookAndFeel_V4
	{
	public:
		PresetButtonLookAndFeel()
		{
			bgColour = Colour::fromRGB(42, 42, 42);
		}

		void drawButtonBackground(Graphics& g, Button& button,
			const Colour& backgroundColour,
			bool isMouseOverButton,
			bool isButtonDown) override
		{
			// draw flat background colour without any rounded borders
			g.setColour(bgColour);
			g.fillAll();
		}

	private:

		juce::Colour bgColour;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetButtonLookAndFeel)
	} mainButtonLookAndFeel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetButton)
};


class PresetCB : public juce::ComboBox
{
public:
	PresetCB() : juce::ComboBox("")
	{
		setLookAndFeel(&mainCBLookAndFeel);
	}

	~PresetCB()
	{
		setLookAndFeel(nullptr);
	}

private:
	class PresetCBLookAndFeel : public juce::LookAndFeel_V4
	{
	public:
		PresetCBLookAndFeel()
		{
			bgColour = Colour::fromRGB(42, 42, 42);
		}

		void drawComboBox(Graphics& g, int width, int height, bool isButtonDown,
			int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box) override
		{
			// Define corner radius
        

			// draw rounded background colour
			g.setColour(bgColour);
			Rectangle<float> backgroundRect(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
			g.fillAll();

			// draw the icon as white
			g.setColour(Colours::white);
			Path path;
			path.startNewSubPath(0.0f, 0.0f);
			path.lineTo(8.0f, 0.0f);
			path.lineTo(4.0f, 8.0f);
			path.closeSubPath();
			g.fillPath(path, AffineTransform::translation(width - 12.0f, (height - 8.0f) * 0.5f));
		}

		// do the same for the dropdown menu

		void drawPopupMenuBackground(Graphics& g, int width, int height) override
		{
			g.setColour(bgColour);
			g.fillAll();
		}

		void      // change the highlight colour
			drawPopupMenuItem(Graphics& g, const juce::Rectangle<int>& area,
				bool isSeparator, bool isActive,
				bool isHighlighted, bool isTicked,
				bool hasSubMenu, const String& text,
				const String& shortcutKeyText,
				const Drawable* icon, const Colour* const textColourToUse) override
		{
			if (isSeparator)
			{
				Rectangle<float> r(area.toFloat().reduced(0, 7));
				r.removeFromTop(r.getHeight() * 0.5f - 0.5f);

				g.setColour(bgColour);
				g.fillRect(r.removeFromTop(1));

				ColourGradient cg(bgColour, 0.0f, 0.0f,
					bgColour.brighter(0.1f), 0.0f, (float)area.getHeight(), false);

				g.setGradientFill(cg);
				g.fillRect(r.removeFromTop(1));
			}
			else
			{
				Rectangle<int> r(area);

				if (isHighlighted)
				{
					g.setColour(bgColour.brighter(0.1f));
					g.fillRect(r);

					g.setColour(bgColour.brighter(0.2f));
					g.drawRect(r, 1);
				}

				g.setColour(bgColour);
				g.setFont(jmin(15.0f, area.getHeight() * 0.85f));

				Rectangle<float> iconArea((float)r.removeFromLeft((r.getHeight() * 5) / 4).reduced(3).getX(),
					(float)r.getY(), (float)r.getHeight(), (float)r.getHeight());

				if (icon != nullptr)
				{
					icon->drawWithin(g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
				}
				else if (isTicked)
				{
					const Path tick(getTickShape(1.0f));
					g.setColour(Colours::white);
					g.fillPath(tick, tick.getTransformToScaleToFit(iconArea.reduced(iconArea.getWidth() / 5, 0).toFloat(), true));
				}

				if (hasSubMenu)
				{
					const auto arrowH = 0.6f * getPopupMenuFont().getHeight();

					const auto x = (float)r.removeFromRight((int)arrowH).getX();
					const auto halfH = (float)r.getCentreY();

					Path path;
					path.startNewSubPath(x, halfH - arrowH * 0.5f);
					path.lineTo(x + arrowH * 0.6f, halfH);
					path.lineTo(x, halfH + arrowH * 0.5f);

					g.strokePath(path, PathStrokeType(2.0f));
				}
				g.setColour(isHighlighted ? Colours::white : Colours::grey);
				g.setFont(getPopupMenuFont());

				g.setColour(Colours::white);
				g.drawText(text, r, Justification::centredLeft, true);
			}
		}






	private:

		juce::Colour bgColour;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetCBLookAndFeel)
	} mainCBLookAndFeel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetCB)
};

class ModeCB : public juce::ComboBox
{
public:
	ModeCB() : juce::ComboBox("")
	{
		setLookAndFeel(&mainCBLookAndFeel);
	}

	~ModeCB()
	{
		setLookAndFeel(nullptr);
	}

private:
	class ModeCBLookAndFeel : public juce::LookAndFeel_V4
	{
	public:
		ModeCBLookAndFeel()
		{
			bgColour = Colour::fromRGB(42, 42, 42);
		}

		void drawComboBox(Graphics& g, int width, int height, bool isButtonDown,
			int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box) override
		{
			// Define corner radius
			float cornerRadius = 5.0f;

			// draw rounded background colour
			g.setColour(bgColour);
			Rectangle<float> backgroundRect(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
			g.fillRoundedRectangle(backgroundRect, cornerRadius);

			// draw the icon as white
			g.setColour(Colours::white);
			Path path;
			path.startNewSubPath(0.0f, 0.0f);
			path.lineTo(8.0f, 0.0f);
			path.lineTo(4.0f, 8.0f);
			path.closeSubPath();
			g.fillPath(path, AffineTransform::translation(width - 12.0f, (height - 8.0f) * 0.5f));
		}

		// do the same for the dropdown menu

		void drawPopupMenuBackground(Graphics& g, int width, int height) override
		{
			g.setColour(bgColour);
			g.fillAll();
		}

		void      // change the highlight colour
			drawPopupMenuItem(Graphics& g, const juce::Rectangle<int>& area,
				bool isSeparator, bool isActive,
				bool isHighlighted, bool isTicked,
				bool hasSubMenu, const String& text,
				const String& shortcutKeyText,
				const Drawable* icon, const Colour* const textColourToUse) override
		{
			if (isSeparator)
			{
				Rectangle<float> r(area.toFloat().reduced(0, 7));
				r.removeFromTop(r.getHeight() * 0.5f - 0.5f);

				g.setColour(bgColour);
				g.fillRect(r.removeFromTop(1));

				ColourGradient cg(bgColour, 0.0f, 0.0f,
					bgColour.brighter(0.1f), 0.0f, (float)area.getHeight(), false);

				g.setGradientFill(cg);
				g.fillRect(r.removeFromTop(1));
			}
			else
			{
				Rectangle<int> r(area);

				if (isHighlighted)
				{
					g.setColour(bgColour.brighter(0.1f));
					g.fillRoundedRectangle(r.toFloat(), 5.0f);

					g.setColour(bgColour.brighter(0.2f));
					g.drawRoundedRectangle(r.toFloat().reduced(0.5f, 0.5f), 5.0f, 1.0f);
				}

				g.setColour(bgColour);
				g.setFont(jmin(15.0f, area.getHeight() * 0.85f));

				Rectangle<float> iconArea((float)r.removeFromLeft((r.getHeight() * 5) / 4).reduced(3).getX(),
					(float)r.getY(), (float)r.getHeight(), (float)r.getHeight());

				if (icon != nullptr)
				{
					icon->drawWithin(g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
				}
				else if (isTicked)
				{
					const Path tick(getTickShape(1.0f));
					g.setColour(Colours::white);
					g.fillPath(tick, tick.getTransformToScaleToFit(iconArea.reduced(iconArea.getWidth() / 5, 0).toFloat(), true));
				}

				if (hasSubMenu)
				{
					const auto arrowH = 0.6f * getPopupMenuFont().getHeight();

					const auto x = (float)r.removeFromRight((int)arrowH).getX();
					const auto halfH = (float)r.getCentreY();

					Path path;
					path.startNewSubPath(x, halfH - arrowH * 0.5f);
					path.lineTo(x + arrowH * 0.6f, halfH);
					path.lineTo(x, halfH + arrowH * 0.5f);

					g.strokePath(path, PathStrokeType(2.0f));
				}
				g.setColour(isHighlighted ? Colours::white : Colours::grey);
				g.setFont(getPopupMenuFont());

				g.setColour(Colours::white);
				g.drawText(text, r, Justification::centredLeft, true);
			}
		}






	private:

		juce::Colour bgColour;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModeCBLookAndFeel)
	} mainCBLookAndFeel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModeCB)
};



class ExtLabel : public Label
{
public:
	ExtLabel() : Label("", "")
	{
		setLookAndFeel(&mainLabelLookAndFeel);
	}

	// click = open link
	void mouseDown(const MouseEvent& event) override
	{
		if (event.mods.isLeftButtonDown())
		{
			URL("https://direktdsp.com").launchInDefaultBrowser();
		}
	}

	~ExtLabel()
	{
		setLookAndFeel(nullptr);
	}

private:
	struct MainLabelLookAndFeel : public LookAndFeel_V4
	{
		MainLabelLookAndFeel()
		{
		}

	private:
		juce::Colour bgColour;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLabelLookAndFeel)
	} mainLabelLookAndFeel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExtLabel)
};
