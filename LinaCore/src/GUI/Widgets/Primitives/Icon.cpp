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

#include "Core/GUI/Widgets/Primitives/Icon.hpp"
#include "Core/Resources/ResourceManager.hpp"
#include "Common/Math/Math.hpp"
#include <LinaGX/Core/InputMappings.hpp>

namespace Lina
{
	void Icon::Initialize()
	{
		CalculateIconSize();
	}

	void Icon::CalculateSize(float dt)
	{
		const float dpiScale = m_lgxWindow->GetDPIScale();
		if (!Math::Equals(dpiScale, m_calculatedDPIScale, 0.01f))
			CalculateIconSize();

		if (m_props.dynamicSizeToParent)
		{
			const Vector2 sz		 = m_parent->GetEndFromMargins() - m_parent->GetStartFromMargins();
			const float	  targetSize = Math::Min(sz.x, sz.y) * m_props.dynamicSizeScale;
			const float	  scale		 = targetSize / static_cast<float>(m_lvgFont->size);
			m_props.textScale		 = scale;
			CalculateIconSize();
		}
	}

	void Icon::Draw()
	{
		if (!GetIsVisible())
			return;

		if (m_lvgFont == nullptr)
			return;

		m_textOptions.color		  = m_props.color.AsLVG();
		m_textOptions.textScale	  = m_props.textScale;
		m_textOptions.cpuClipping = m_props.customClip.AsLVG4();

		if (m_props.enableHoverPressColors)
		{
			if (m_isHovered)
				m_textOptions.color.start = m_textOptions.color.end = m_props.colorHovered.AsLVG4();

			if (m_isPressed)
				m_textOptions.color.start = m_textOptions.color.end = m_props.colorPressed.AsLVG4();
		}

		if (GetIsDisabled())
			m_textOptions.color.start = m_textOptions.color.end = m_props.colorDisabled.AsLVG4();

		m_lvg->DrawText(m_props.icon.c_str(), (m_rect.pos + Vector2(0.0f, m_rect.size.y)).AsLVG(), m_textOptions, 0.0f, m_drawOrder, m_props.isDynamic);

		DrawTooltip();
	}

	void Icon::CalculateIconSize()
	{
		auto*		font	 = m_resourceManager->GetResource<Font>(m_props.font);
		const float dpiScale = m_lgxWindow->GetDPIScale();
		m_lvgFont			 = font->GetFont(dpiScale);

		if (m_lvgFont == nullptr)
			return;

		m_calculatedDPIScale = dpiScale;
		m_textOptions.font	 = m_lvgFont;
		m_rect.size			 = static_cast<float>(Math::RoundToIntEven(m_lvgFont->size * m_props.textScale));
	}

	bool Icon::OnMouse(uint32 button, LinaGX::InputAction act)
	{
		if (button != LINAGX_MOUSE_0)
			return false;

		if ((act == LinaGX::InputAction::Pressed || act == LinaGX::InputAction::Repeated) && m_isHovered && m_props.onClicked)
		{
			m_isPressed = true;
			return true;
		}

		if (act == LinaGX::InputAction::Released)
		{
			if (m_isPressed && m_isHovered)
			{
				if (m_props.onClicked)
					m_props.onClicked();

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
