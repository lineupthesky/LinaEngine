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

#include "Core/GUI/Widgets/Widget.hpp"
#include "Common/Data/String.hpp"
#include "Common/Data/Bitmask.hpp"

namespace Lina
{
	class Text;

	class InputField : public Widget, public LinaGX::WindowListener
	{
	public:
		InputField() : Widget(1)
		{
		}
		virtual ~InputField() = default;

		struct Properties
		{
			Color colorBackground	   = Color::White;
			Color colorOutline		   = Color::White;
			Color colorOutlineControls = Color::White;
			Color colorHighlight	   = Color::White;
			Color colorCaret		   = Color::White;
			Color colorNumberFillStart = Color::White;
			Color colorNumberFillEnd   = Color::White;
			float rounding			   = 0.0f;
			float outlineThickness	   = 0.0f;
			float indent			   = 0.0f;

			bool   isNumberField	   = false;
			bool   disableNumberSlider = false;
			bool   clampNumber		   = false;
			float* numberValue		   = nullptr;
			float  numberMin		   = 0.0f;
			float  numberMax		   = 10.0f;
			float  numberStep		   = 0.0f;
			uint32 numberPrecision	   = 3;
		};

		virtual void Construct() override;
		virtual void Destruct() override;
		virtual void Tick(float delta) override;
		virtual void Draw(int32 threadIndex) override;
		virtual void OnWindowKey(uint32 keycode, int32 scancode, LinaGX::InputAction action) override;
		virtual void OnWindowMouse(uint32 button, LinaGX::InputAction action) override;

		inline Properties& GetProps()
		{
			return m_props;
		}

		inline Text* GetText()
		{
			return m_text;
		}

	private:
		void	SelectAll();
		void	EndEditing();
		uint32	GetCaretPosFromMouse();
		Vector2 GetPosFromCaretIndex(uint32 index);
		float	GetCaretStartY();
		float	GetCaretEndY();
		void	Insert(uint32 pos, const String& str);
		void	RemoveCurrent();
		void	ClampCaretInsert();

	private:
		Properties m_props	   = {};
		Text*	   m_text	   = nullptr;
		bool	   m_isEditing = false;

		uint32 m_caretInsertPos	   = 0;
		uint32 m_highlightStartPos = 0;

		float	m_caretAlpha		   = 1.0f;
		float	m_caretCtr			   = 0.0f;
		Vector2 m_textStartMid		   = Vector2::Zero;
		Vector2 m_textEndMid		   = Vector2::Zero;
		float	m_averageCharacterStep = 0.0f;
		float	m_textOffset		   = 0.0f;
	};

} // namespace Lina