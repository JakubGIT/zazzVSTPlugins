/*
 * Copyright (C) 2025 Filip Cenzak (filip.c@centrum.cz)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <JuceHeader.h>

class ModernRotarySliderTextBox : public juce::Label
{
public:
	ModernRotarySliderTextBox(const juce::String& componentName, const juce::String& initialText) : juce::Label(componentName, initialText)
	{
		setEditable(true, true, false);																// Allow editing on single or double-click
		setJustificationType(juce::Justification::centred);
		setColour(juce::Label::backgroundColourId, juce::Colours::lightgrey);
		setColour(juce::Label::textColourId, juce::Colours::white);
		setColour(juce::Label::outlineColourId, juce::Colours::darkgrey);
		setColour(juce::Label::outlineWhenEditingColourId, juce::Colour::fromRGB(90, 90, 100));
	}

	void textWasEdited() override
	{
		// Handle the text change when the user finishes editing
		// You can add additional logic here, such as notifying a listener
	}

	void paint(juce::Graphics& g) override
	{
		//g.fillAll(juce::Colour::fromRGB(90, 90, 100));
		
		//g.setFont(0.8f * static_cast<float>(getHeight()));
		//g.setColour(juce::Colours::white);
		//g.drawText(getText(), getLocalBounds(), getJustificationType(), true);
	}

	// Override createEditorComponent to customize the TextEditor
	juce::TextEditor* createEditorComponent() override
	{
		auto* editor = new juce::TextEditor();

		// Set the font and justification for the TextEditor
		editor->setFont(getFont());  // Set font size to match the label
		editor->setJustification(getJustificationType());  // Apply justification during editing
		
		// Set the background color of the TextEditor
		editor->setColour(juce::TextEditor::backgroundColourId, juce::Colour::fromRGB(90, 90, 100));

		// Set the outline color of the TextEditor
		editor->setColour(juce::TextEditor::outlineColourId, juce::Colour::fromRGB(90, 90, 100));
		editor->setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour::fromRGB(90, 90, 100));

		return editor;
	}
};

class SmallSliderLookaAndFeel : public juce::LookAndFeel_V4
{
public:
	SmallSliderLookaAndFeel() = default;

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
	{
		const auto widthHalf = 0.5f * static_cast<float>(width);
		const auto heightHalf = 0.5f * static_cast<float>(height);
		const auto radius = 0.8f * widthHalf;
		const auto centreX = static_cast<float>(x) + widthHalf;
		const auto centreY = static_cast<float>(y) + heightHalf;
		const auto rx = centreX - radius;
		const auto ry = centreY - radius;
		const auto rw = 2.0f * radius;
		const auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		juce::Rectangle<float> bounds(rx, ry, rw, rw);

		// Light outer circle
		g.setColour(lightColor);
		g.fillEllipse(bounds);
		
		// Dark outer stroke with gradiesnt	
		float expandPixels = 0.004f * rw;
		bounds.expand(-expandPixels, -expandPixels);
		g.setColour(darkColor);
		g.drawEllipse(bounds, expandPixels);

		// Light to dark circular gradient		
		bounds.expand(-expandPixels, -expandPixels);
		juce::ColourGradient gradient(darkColor, centreX, centreY, lightColor, centreX + 0.5f * bounds.getWidth(), centreY + 0.5f * bounds.getWidth(), true);
		gradient.addColour(0.1f, darkColor);
		g.setGradientFill(gradient);
		g.fillEllipse(bounds);

		// Dark to light gradient
		expandPixels = bounds.getWidth() / 2.0f / 2.7f;
		bounds.expand(-expandPixels, -expandPixels);
		juce::ColourGradient gradient2(lightColor, centreX, centreY, darkColor, centreX + 0.5f * bounds.getWidth(), centreY + 0.5f * bounds.getWidth(), true);
		gradient2.addColour(0.8f, darkColor);
		g.setGradientFill(gradient2);
		g.fillEllipse(bounds);

		// Light inner ring
		expandPixels = 0.004f * rw;
		bounds.expand(-expandPixels, -expandPixels);
		g.setColour(lightColor);
		g.drawEllipse(bounds, expandPixels);

		// Dark center circle
		expandPixels = bounds.getWidth() / 2.0f / 4.3f;
		bounds.expand(-expandPixels, -expandPixels);
		g.setColour(darkColor);
		g.fillEllipse(bounds);

		// Light outer ring
		expandPixels = 0.008f * rw;
		g.setColour(lightColor);
		g.drawEllipse(bounds, expandPixels);

		// Create background arc path
		const float arcRadius = 0.8f * radius;
		juce::Path arcPath;
		arcPath.addCentredArc(centreX, centreY, arcRadius, arcRadius,
			0.0f,																	// Rotation
			rotaryStartAngle,														// Start angle
			rotaryEndAngle,															// End angle
			true);																	// UseAsSegment

		const float strokeThicknessBeckground = 0.18f * radius;
		juce::PathStrokeType strokeTypeBackground(strokeThicknessBeckground);
		strokeTypeBackground.setEndStyle(juce::PathStrokeType::EndCapStyle::rounded);
		
		g.setColour(darkColor);
		g.strokePath(arcPath, strokeTypeBackground);

		// Create the arc path for set value
		arcPath.clear();
		arcPath.addCentredArc(centreX, centreY, arcRadius, arcRadius,
			0.0f,																	// Rotation
			rotaryStartAngle,														// Start angle																					
			angle,																	// End angle
			true);																	// UseAsSegment

		const float strokeThicknessActive = 0.16f * radius;
		juce::PathStrokeType strokeTypeActive(strokeThicknessActive);
		strokeTypeActive.setEndStyle(juce::PathStrokeType::EndCapStyle::rounded);
				
		g.setColour(highlightColor);
		g.strokePath(arcPath, strokeTypeActive);

		// Knob point line
		juce::Path markerPath;
		juce::Line<float> line(0.0f, -0.7f * 0.5f * bounds.getWidth(), 0.0f, -0.4f * 0.5f * bounds.getWidth());
		
		markerPath.startNewSubPath(line.getStart());
		markerPath.lineTo(line.getEnd());
		markerPath.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
		
		g.strokePath(markerPath, strokeTypeActive);

		//juce::Colour shadowColor = highlightColor.withAlpha(0.5f);
		//juce::DropShadow dropShadowEffect(shadowColor, 0.1f * radius, { 0, 0 });

		//dropShadowEffect.drawForPath(g, arcPath);

		//juce::Image tempImage(juce::Image::ARGB, width, height , true);

		// Render marker to the image
		//juce::Graphics tempGraphics(tempImage);

		//tempGraphics.setColour(highlightColor);
		//tempGraphics.strokePath(arcPath, strokeTypeActive);
		//tempGraphics.strokePath(markerPath, strokeTypeActive);

		// Apply drop shaddow effect
		//dropShadowEffect.drawForImage(tempGraphics, tempImage);

		// Render to graphic
		//g.drawImageAt(tempImage, 0, 0, true);


	}

	juce::Colour darkColor = juce::Colour::fromRGB(40, 42, 46);
	juce::Colour lightColor = juce::Colour::fromRGB(68, 68, 68);
	juce::Colour highlightColor = juce::Colour::fromRGB(255, 255, 190);
};

//==============================================================================

class ModernRotarySlider : public juce::Component, private juce::Slider::Listener
{
public:
	ModernRotarySlider(/*juce::AudioProcessorValueTreeState& vts, const std::string name, */const std::string unit, const std::string label) : /*valueTreeState(vts), m_name(name), */m_unit(unit), m_label(label)
	{
		// Use custom lookAndFeel
		m_slider.setLookAndFeel(&m_smallSliderLookAndFeel);
		
		// Create rotary slider
		m_slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
		m_slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
		m_slider.addListener(this);
		addAndMakeVisible(m_slider);

		// Attach slider to valueTreeState
		//m_sliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, name, m_slider));
		
		// Create text box
		//m_textBox.setJustificationType(juce::Justification::centred);
		//m_textBox.setEditable(true, true, false);														// Allow editing
		
		//m_textBox.setText(juce::String(m_slider.getValue()) + unit, juce::dontSendNotification);		// Initialize with slider value
		//m_textBox.onTextChange = [this]() { handleTextBoxChange(); };									// Update slider on label change
		//addAndMakeVisible(m_textBox);
	};
	~ModernRotarySlider()
	{
		m_slider.removeListener(this);
	};

	static const int MICRO_LOD_WIDTH_LIMIT = 50;
	static const int SMALL_LOD_WIDTH_LIMIT = 80;

	inline void paint(juce::Graphics& g) override
	{
		g.fillAll(darkColor);

		// Slider name bounds
		const auto width = getWidth();
		const auto height = getHeight();

		if (width > MICRO_LOD_WIDTH_LIMIT)
		{
			juce::Rectangle<int> bounds;
			bounds.setPosition(0, 5 * height / 100);
			bounds.setSize(width, 15 * height / 100);

			// Slider name
			//g.setColour(juce::Colours::white);
			//g.setFont(0.8f * bounds.getHeight());
			//g.drawText(m_label, bounds, juce::Justification::centred, false);
		}
	}
	inline void resized() override
	{
		// Size: 3 x 4
		
		const auto width = getWidth();
		const auto height = getHeight();

		if (width > SMALL_LOD_WIDTH_LIMIT)
		{
			juce::Rectangle<int> bounds;
			bounds.setPosition(0, 20 * height / 100);
			bounds.setSize(width, 60 * height / 100);

			m_slider.setBounds(bounds);

			bounds.setPosition(0, 80 * height / 100);
			bounds.setSize(width, 15 * height / 100);
			
			if (!m_textBox.isVisible())
			{
				m_textBox.setVisible(true);
			}

			m_textBox.setBounds(bounds);
		}
		else if (width > MICRO_LOD_WIDTH_LIMIT)
		{
			juce::Rectangle<int> bounds;
			bounds.setPosition(0, 20 * height / 100);
			bounds.setSize(width, 70 * height / 100);

			m_slider.setBounds(bounds);

			if (m_textBox.isVisible())
			{
				m_textBox.setVisible(false);
			}
		}
		else
		{
			juce::Rectangle<int> bounds;
			bounds.setPosition(0, 5 * height / 100);
			bounds.setSize(width, 90 * height / 100);

			m_slider.setBounds(bounds);

			if (m_textBox.isVisible())
			{
				m_textBox.setVisible(false);
			}
		}
	}

private:
	// Update label when slider changes
	void sliderValueChanged(juce::Slider* sliderThatChanged) override
	{
		if (sliderThatChanged == &m_slider)
		{
			const auto value = m_slider.getValue();
			m_textBox.setText(juce::String(value) + m_unit, juce::dontSendNotification);
		}
	}

	// Update slider when label text changes
	void handleTextBoxChange()
	{
		auto enteredText = m_textBox.getText();

		// Attempt to parse the text as a double
		double newValue = enteredText.getDoubleValue();

		// Validate that the entered text is a valid number
		bool isValidNumber = enteredText.trim().isNotEmpty() && enteredText == juce::String(newValue);

		if (!isValidNumber)
		{
			return;
		}

		m_slider.setValue(newValue, juce::sendNotification);      // Update slider value
	}

	//juce::AudioProcessorValueTreeState& valueTreeState;
	SmallSliderLookaAndFeel m_smallSliderLookAndFeel;

	ModernRotarySliderTextBox m_textBox{ "CustomLabel", "Default Text" };

	juce::Slider m_slider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_sliderAttachment;
	
	juce::String m_name;
	juce::String m_unit;
	juce::String m_label;

	juce::Colour darkColor = juce::Colour::fromRGB(40, 42, 46);
	juce::Colour lightColor = juce::Colour::fromRGB(68, 68, 68);
};
