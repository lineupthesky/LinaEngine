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

#include "GUI/Nodes/Panels/GUINodePanel.hpp"
#include "Graphics/Platform/LinaVGIncl.hpp"
#include "GUI/Utility/GUIUtility.hpp"
#include "GUI/Nodes/Custom/GUINodeDockPreview.hpp"
#include "GUI/Nodes/Widgets/GUINodeTab.hpp"

namespace Lina::Editor
{
	GUINodePanel::GUINodePanel(Editor* editor, ISwapchain* swapchain, int drawOrder, const String& title, GUINodeDockArea* parentDockArea) : m_title(title), m_parentDockArea(parentDockArea), GUINode(editor, swapchain, drawOrder)
	{
		m_dockPreview = new GUINodeDockPreview(editor, swapchain, drawOrder);
		m_tab		  = new GUINodeTab(editor, swapchain, drawOrder);
		m_tab->SetCallbackClose([this]() {
			m_parentDockArea->OnPanelClosed(this);
		});
		AddChildren(m_dockPreview)->AddChildren(m_tab);
	}

	void GUINodePanel::Draw(int threadID)
	{
		GUIUtility::DrawPanelBackground(threadID, m_rect, m_drawOrder);
	}

	void GUINodePanel::DrawDockPreview(int threadID)
	{
		m_dockPreview->SetRect(m_rect);
		m_dockPreview->Draw(threadID);
	}
} // namespace Lina::Editor
