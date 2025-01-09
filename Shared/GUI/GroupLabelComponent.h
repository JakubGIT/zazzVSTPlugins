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

class GroupLabelComponent : public juce::Component
{

public:
	GroupLabelComponent(juce::String name) : m_name(name)
	{
	}
	~GroupLabelComponent() = default;

	static const int MICRO_LOD_HEIGHT_LIMIT = 17;

	inline void paint(juce::Graphics& g) override
	{
		// Size: 1 x n
		
		g.fillAll(juce::Colour::fromRGB(90, 90, 100));

		const auto width = getWidth();
		const auto height = getHeight();

		if (height > MICRO_LOD_HEIGHT_LIMIT)
		{
			juce::Rectangle<int> bounds;
			bounds.setPosition(0, 0);
			bounds.setSize(width, height);

			// Group label name
			g.setColour(juce::Colours::white);
			g.setFont(0.4f * static_cast<float>(height));
			g.setOpacity(0.5f);
			g.drawText(m_name, bounds, juce::Justification::centred, false);


			// Draw lines
			int textWidth = g.getCurrentFont().getStringWidth(m_name) + height / 2;

			int yPosition = height / 2;

			int xLeftStart = height / 2;
			int xLeftStop = (width - textWidth) / 2;
			int xRightStart = (width + textWidth) / 2;
			int xRightStop = width - (height / 2);

			g.setOpacity(1.0f);
			g.drawLine(xLeftStart, yPosition, xLeftStop, yPosition, 0.5f);			// Line width = 1.0f
			g.drawLine(xRightStart, yPosition, xRightStop, yPosition, 0.5f);		// Line width = 1.0f
		}
		else
		{
			int yPosition = height / 2;

			int xStart = height / 2;
			int xStop = width - (height / 2);
			
			g.setColour(juce::Colours::white);
			g.drawLine(xStart, yPosition, xStop, yPosition, 0.5f);			// Line width = 1.0f
		}
	}

	inline void resized() override
	{

	}

private:
	juce::String m_name;
};