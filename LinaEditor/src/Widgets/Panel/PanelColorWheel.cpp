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

#include "Editor/Widgets/Panel/PanelColorWheel.hpp"
#include "Editor/Widgets/Compound/ColorWheelCompound.hpp"
#include "Core/GUI/Widgets/WidgetManager.hpp"

namespace Lina::Editor
{
	void PanelColorWheel::Construct()
	{
		ColorWheelCompound* cw = m_manager->Allocate<ColorWheelCompound>();
		cw->GetFlags().Set(WF_POS_ALIGN_X | WF_POS_ALIGN_Y | WF_SIZE_ALIGN_X | WF_SIZE_ALIGN_Y);
		cw->SetAlignedPos(Vector2::Zero);
		cw->SetAlignedSize(Vector2::One);
		AddChild(cw);
		m_colorWheel = cw;
	}

	void PanelColorWheel::SetTarget(Color* color)
	{
		m_colorWheel->GetProps().onValueChanged = [color](const Color& linearColor) { *color = linearColor; };
		m_colorWheel->SetTargetColor(*color);
	}
} // namespace Lina::Editor