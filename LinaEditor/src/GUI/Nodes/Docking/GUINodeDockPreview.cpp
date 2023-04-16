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

#include "GUI/Nodes/Docking/GUINodeDockPreview.hpp"
#include "Graphics/Platform/LinaVGIncl.hpp"
#include "Graphics/Resource/Texture.hpp"
#include "Core/Editor.hpp"
#include "Core/Theme.hpp"
#include "GUI/Utility/GUIUtility.hpp"
#include "Graphics/Interfaces/ISwapchain.hpp"
#include "System/ISystem.hpp"
#include "Input/Core/Input.hpp"
#include "Graphics/Interfaces/IWindow.hpp"
#include "Math/Math.hpp"

namespace Lina::Editor
{
	GUINodeDockPreview::GUINodeDockPreview(Editor* editor, ISwapchain* swapchain, int drawOrder) : GUINode(editor, swapchain, drawOrder)
	{
		m_input = editor->GetSystem()->CastSubsystem<Input>(SubsystemType::Input);
	}

	void GUINodeDockPreview::Draw(int threadID)
	{
		if (!m_visible)
			return;

		auto mousePos = m_input->GetMousePositionAbs() - m_window->GetPos();

		const Vector2 panelCenter = Vector2(m_rect.pos.x + m_rect.size.x * 0.5f, m_rect.pos.y + m_rect.size.y * 0.5f);
		m_currentHoveredSplit	  = DockSplitType::None;

		auto drawDockArea = [&](uint32 imageIndex, DockSplitType splitType, const Vector2& direction) {
			const TextureSheetItem& item	 = m_editor->GetEditorImage(imageIndex);
			const Vector2i			rectSize = item.size * 0.14f;
			const float				padding	 = rectSize.x * 1.2f;
			const Vector2i			itemSize = item.size * 0.1f;

			Color col = Theme::TC_CyanAccent;
			col.w	  = 0.3f;

			LinaVG::StyleOptions opts;
			opts.color	   = LV4(col);
			opts.rounding  = 0.2f;
			opts.aaEnabled = true;

			Vector2 center = Math::Lerp(panelCenter, panelCenter + m_rect.size * 0.5f * direction, 0.8f);

			if (m_isOuter)
			{
				center = Vector2(panelCenter.x + m_rect.size.x * 0.5f * direction.x, panelCenter.y + m_rect.size.y * 0.5f * direction.y);
				center.x -= itemSize.x * direction.x;
				center.y -= itemSize.y * direction.y;
			}

			const Rect splitAreaRect = Rect(Vector2(center.x - rectSize.x * 0.5f, center.y - rectSize.y * 0.5f), rectSize);

			LinaVG::DrawRect(threadID, LV2(splitAreaRect.pos), LV2((splitAreaRect.pos + splitAreaRect.size)), opts, 0.0f, FRONT_DRAW_ORDER);
			GUIUtility::DrawSheetImage(threadID, item, (splitAreaRect.pos + splitAreaRect.pos + splitAreaRect.size) * 0.5f, itemSize, Color::White, m_drawOrder);

			if (splitAreaRect.IsPointInside(mousePos))
			{
				LinaVG::StyleOptions rectOpts;
				rectOpts.color = LV4(col);

				m_currentHoveredSplit = splitType;

				Vector2 rectStartPos = Vector2::Zero;
				Vector2 rectSize	 = Vector2::Zero;

				if (m_currentHoveredSplit == DockSplitType::Left)
				{
					rectStartPos = m_rect.pos;
					rectSize	 = Vector2(m_rect.size.x * EDITOR_DEFAULT_DOCK_SPLIT, m_rect.size.y);
				}
				else if (m_currentHoveredSplit == DockSplitType::Right)
				{
					rectStartPos = Vector2(m_rect.pos.x + m_rect.size.x - m_rect.size.x * EDITOR_DEFAULT_DOCK_SPLIT, m_rect.pos.y);
					rectSize	 = Vector2(m_rect.size.x * EDITOR_DEFAULT_DOCK_SPLIT, m_rect.size.y);
				}
				else if (m_currentHoveredSplit == DockSplitType::Up)
				{
					rectStartPos = m_rect.pos;
					rectSize	 = Vector2(m_rect.size.x, m_rect.size.y * EDITOR_DEFAULT_DOCK_SPLIT);
				}
				else if (m_currentHoveredSplit == DockSplitType::Down)
				{
					rectStartPos = Vector2(m_rect.pos.x, m_rect.pos.y + m_rect.size.y - m_rect.size.y * EDITOR_DEFAULT_DOCK_SPLIT);
					rectSize	 = Vector2(m_rect.size.x, m_rect.size.y * EDITOR_DEFAULT_DOCK_SPLIT);
				}

				LinaVG::DrawRect(threadID, LV2(rectStartPos), LV2((rectStartPos + rectSize)), rectOpts, 0.0f, FRONT_DRAW_ORDER);
			}
		};

		drawDockArea(m_isOuter ? EDITOR_IMAGE_DOCK_OUTER_LEFT : EDITOR_IMAGE_DOCK_LEFT, DockSplitType::Left, Vector2(-1.0f, 0.0f));
		drawDockArea(m_isOuter ? EDITOR_IMAGE_DOCK_OUTER_RIGHT : EDITOR_IMAGE_DOCK_RIGHT, DockSplitType::Right, Vector2(1.0f, 0.0f));
		drawDockArea(m_isOuter ? EDITOR_IMAGE_DOCK_OUTER_UP : EDITOR_IMAGE_DOCK_UP, DockSplitType::Up, Vector2(0.0f, -1.0f));
		drawDockArea(m_isOuter ? EDITOR_IMAGE_DOCK_OUTER_DOWN : EDITOR_IMAGE_DOCK_DOWN, DockSplitType::Down, Vector2(0.0f, 1.0f));
	}
} // namespace Lina::Editor
