/*
This file is a part of: Lina Engine
https://github.com/inanevin/LinaEngine

Author: Inan Evin
http://www.inanevin.com

Copyright (c) [2018-] [Inan Evin]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#ifndef GUINodeButton_HPP
#define GUINodeButton_HPP

#include "GUI/Nodes/GUINode.hpp"
#include "Data/String.hpp"
#include "Core/Theme.hpp"
#include "Data/Functional.hpp"

namespace Lina::Editor
{

	enum class ButtonFitType
	{
		None,
		AutoFitFromTextAndPadding,
	};

	class GUINodeButton : public GUINode
	{
	public:
		GUINodeButton(Editor* editor, ISwapchain* swapchain, int drawOrder) : GUINode(editor, swapchain, drawOrder){};
		virtual ~GUINodeButton() = default;

		virtual void	Draw(int threadID) override;
		virtual void	OnClicked(uint32 button) override;
		virtual Vector2 CalculateSize() override;

		inline GUINodeButton* SetFontType(FontType ft)
		{
			m_fontType = ft;
			return this;
		}

		inline GUINodeButton* SetText(const char* text)
		{
			m_text = text;
			return this;
		}

		inline GUINodeButton* SetDefaultColor(const Color& col)
		{
			m_defaultColor = col;
			return this;
		}

		inline GUINodeButton* SetHoveredColor(const Color& col)
		{
			m_hoveredColor = col;
			return this;
		}

		inline GUINodeButton* SetPressedColor(const Color& col)
		{
			m_pressedColor = col;
			return this;
		}

		inline GUINodeButton* SetOutlineColor(const Color& col)
		{
			m_outlineColor = col;
			return this;
		}

		inline GUINodeButton* SetEnableHoverOutline(bool enableOutline)
		{
			m_enableHoverOutline = enableOutline;
			return this;
		}

		inline GUINodeButton* SetFitType(ButtonFitType ft)
		{
			m_fitType = ft;
			return this;
		}

		inline GUINodeButton* SetCallback(Delegate<void(GUINodeButton*)>&& onClicked)
		{
			m_onClicked = onClicked;
			return this;
		}

	protected:
		float						   m_lastDPI			= 0.0f;
		bool						   m_enableHoverOutline = false;
		Vector2						   m_lastTextSize		= Vector2::Zero;
		ButtonFitType				   m_fitType			= ButtonFitType::AutoFitFromTextAndPadding;
		FontType					   m_fontType			= FontType::DefaultEditor;
		String						   m_text				= "";
		Color						   m_defaultColor		= Color::White;
		Color						   m_hoveredColor		= Color::Gray;
		Color						   m_pressedColor		= Color::Black;
		Color						   m_outlineColor		= Color::Black;
		Delegate<void(GUINodeButton*)> m_onClicked;
	};

	class GUINodeButtonIcon : public GUINodeButton
	{
	public:
		GUINodeButtonIcon(Editor* editor, ISwapchain* swapchain, int drawOrder) : GUINodeButton(editor, swapchain, drawOrder){};
		virtual ~GUINodeButtonIcon() = default;

		virtual void Draw(int threadID) override;

		inline GUINodeButtonIcon* SetIconColor(const Color& col)
		{
			m_iconColor = col;
		}

	private:
		Color m_iconColor = Color::White;
	};
} // namespace Lina::Editor

#endif
