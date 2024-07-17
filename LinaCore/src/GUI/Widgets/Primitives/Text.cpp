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

#include "Core/GUI/Widgets/Primitives/Text.hpp"
#include "Core/Graphics/Resource/Font.hpp"
#include "Core/Resources/ResourceManager.hpp"
#include "Common/System/SystemInfo.hpp"
#include "Common/Math/Math.hpp"
#include <LinaGX/Core/InputMappings.hpp>

namespace Lina
{
	void Text::Initialize()
	{
		CalculateTextSize();
	}

	void Text::CalculateSize(float delta)
	{
		if (m_props.fetchWrapFromParent)
			m_props.wrapWidth = m_parent->GetSizeX();

		if (!Math::Equals(m_lgxWindow->GetDPIScale(), m_calculatedDPIScale, 0.01f))
			CalculateTextSize();
	}

	void Text::Draw()
	{
		if (!GetIsVisible())
			return;

		if (!Math::Equals(m_lgxWindow->GetDPIScale(), m_calculatedDPIScale, 0.01f))
			CalculateTextSize();

		if (m_props.fetchCustomClipFromParent && m_parent)
		{
			const Vector2 start = m_parent->GetStartFromMargins();
			const Vector2 end	= m_parent->GetEndFromMargins();
			m_props.customClip	= Vector4(start.x, start.y, end.x - start.x, end.y - start.y + 10.0f);
		}

		if (m_props.fetchCustomClipFromSelf)
		{
			m_props.customClip = Vector4(GetPosX() - GetSizeX() * 0.1f, GetPosY() - GetSizeY() * 0.5f, GetSizeX() * 1.2f, GetSizeY() * 2.0f);
		}

		if (m_waitingOnClickedDelay)
		{
			if (SystemInfo::GetAppTimeF() - m_lastPressSeconds > 0.2)
			{
				if (m_props.onClicked)
					m_props.onClicked();
				m_waitingOnClickedDelay = false;
			}
		}

		if (m_isSDF)
		{
			LinaVG::SDFTextOptions opts;
			opts.font		 = m_lvgFont;
			opts.textScale	 = m_props.textScale;
			opts.alignment	 = m_props.alignment;
			opts.wrapWidth	 = m_props.wrapWidth;
			opts.color.start = opts.color.end = GetIsDisabled() ? m_props.colorDisabled.AsLVG4() : m_props.color.AsLVG4();
			opts.cpuClipping				  = m_props.customClip.AsLVG4();
			opts.wordWrap					  = m_props.wordWrap;

			if (GetIsDisabled())
				opts.color = m_props.colorDisabled.AsLVG4();

			m_lvg->DrawTextSDF(m_props.text.c_str(), (m_rect.pos + Vector2(0.0f, m_rect.size.y)).AsLVG(), opts, 0.0f, m_drawOrder, m_props.isDynamic);
		}
		else
		{
			LinaVG::TextOptions opts;
			opts.font			= m_lvgFont;
			opts.textScale		= m_props.textScale;
			opts.alignment		= m_props.alignment;
			opts.wrapWidth		= m_props.wrapWidth;
			opts.newLineSpacing = 0.0f;
			opts.color.start = opts.color.end = GetIsDisabled() ? m_props.colorDisabled.AsLVG4() : m_props.color.AsLVG4();
			opts.cpuClipping				  = m_props.customClip.AsLVG4();
			opts.wordWrap					  = m_props.wordWrap;

			if (GetIsDisabled())
				opts.color = m_props.colorDisabled.AsLVG4();

			auto p = (m_rect.pos + Vector2(0.0f, m_rect.size.y));
			m_lvg->DrawTextNormal(m_props.text.c_str(), p.AsLVG(), opts, 0.0f, m_drawOrder, m_props.isDynamic);
		}
	}

	void Text::CalculateTextSize()
	{
		const float dpiScale = m_lgxWindow->GetDPIScale();
		auto*		font	 = m_resourceManager->GetResource<Font>(m_props.font);
		m_lvgFont			 = font->GetLinaVGFont(dpiScale);
		m_calculatedDPIScale = dpiScale;

		m_isSDF = m_lvgFont->m_isSDF;

		if (m_props.fetchWrapFromParent)
			m_props.wrapWidth = m_parent->GetSizeX();

		if (m_isSDF)
		{
			LinaVG::SDFTextOptions opts;
			opts.font	   = m_lvgFont;
			opts.textScale = m_props.textScale;
			opts.wrapWidth = m_props.wrapWidth;
			opts.wordWrap  = m_props.wordWrap;
			m_rect.size	   = m_lvg->CalculateTextSize(m_props.text.c_str(), opts);
		}
		else
		{
			LinaVG::TextOptions opts;
			opts.font	   = m_lvgFont;
			opts.textScale = m_props.textScale;
			opts.wrapWidth = m_props.wrapWidth;
			opts.wordWrap  = m_props.wordWrap;
			m_rect.size	   = m_lvg->CalculateTextSize(m_props.text.c_str(), opts);
		}
	}

	bool Text::OnMouse(uint32 button, LinaGX::InputAction act)
	{
		if (button != LINAGX_MOUSE_0)
			return false;

		if (m_isHovered)
			m_waitingOnClickedDelay = false;

		if (act == LinaGX::InputAction::Pressed && m_isHovered)
		{
			m_isPressed = true;
			return true;
		}

		if (act == LinaGX::InputAction::Released)
		{
			if (m_isPressed && m_isHovered)
			{
				if (m_props.delayOnClicked)
				{
					m_lastPressSeconds		= SystemInfo::GetAppTimeF();
					m_waitingOnClickedDelay = true;
				}
				else
				{
					if (m_props.onClicked)
						m_props.onClicked();
				}

				m_isPressed = false;
				return true;
			}

			if (m_isPressed)
			{
				m_isPressed = false;
				return true;
			}
		}

		return false;
	}
} // namespace Lina
