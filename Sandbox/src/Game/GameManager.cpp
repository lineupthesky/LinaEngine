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

#include "Game/GameManager.hpp"

#include "Core/Application.hpp"
#include "EventSystem/MainLoopEvents.hpp"
#include "EventSystem/GraphicsEvents.hpp"
#include "Levels/ExampleLevel.hpp"
#include "Log/Log.hpp"
#include "ECS/Components/EntityDataComponent.hpp"
#include "Core/LinaGUI.hpp"
#include "Core/LinaGUIDrawer.hpp"

GameManager* GameManager::s_instance = nullptr;

void GameManager::Initialize()
{
    LINA_TRACE("Game Manager Attached!");
    s_instance = this;
    // m_exampleLevel = new ExampleLevel();
    // Application::Get()->InstallLevel(*m_exampleLevel, true, "Resources/Sandbox/Levels/", "ExampleLevel");
    Event::EventSystem::Get()->Connect<Event::ETick, &GameManager::OnTick>(this);
    Event::EventSystem::Get()->Connect<Event::EShutdown, &GameManager::OnShutdown>(this);
    Event::EventSystem::Get()->Connect<Event::EPostRender, &GameManager::OnGUIRender>(this);

    LGOptions opts;
    const Vector2  screenSize = Graphics::RenderEngineBackend::Get()->GetScreenSize();
    opts.m_displaySize        = LGVec2(screenSize.x, screenSize.y);
    opts.m_framebufferScale   = LGVec2(Graphics::WindowBackend::Get()->GetProperties().m_contentScaleWidth, Graphics::WindowBackend::Get()->GetProperties().m_contentScaleHeight);
    GUI::Initialize(opts);
}

void GameManager::OnGUIRender(const Event::EPostRender& ev)
{
   GUI::Start();
  // Drawer::DrawLine(LGVec2(-0.2f, 0.5f), LGVec2(0.2f, 0.5f), LGVec4(1,1,1,1), 5);
   Drawer::DrawLine(LGVec2(0, 500), LGVec2(3800, 500), LGVec4(1, 1, 1, 1), 25);

   GUI::Render();
   GUI::End();
}

void GameManager::OnTick(const Event::ETick& ev)
{
}

void GameManager::OnShutdown(const Event::EShutdown& ev)
{
    // delete m_exampleLevel;
}
