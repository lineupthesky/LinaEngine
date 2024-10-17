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

#include "Editor/Editor.hpp"
#include "Editor/EditorLocale.hpp"
#include "Editor/Resources/EditorResources.hpp"
#include "Editor/Widgets/Screens/SplashScreen.hpp"
#include "Editor/Widgets/EditorRoot.hpp"
#include "Editor/Graphics/WorldRendererExtEditor.hpp"
#include "Editor/Graphics/SurfaceRenderer.hpp"
#include "Editor/Widgets/CommonWidgets.hpp"
#include "Common/FileSystem/FileSystem.hpp"
#include "Common/Serialization/Serialization.hpp"
#include "Core/Application.hpp"
#include "Core/GUI/Widgets/Primitives/Text.hpp"
#include "Core/Resources/ResourceManager.hpp"
#include "Core/World/EntityWorld.hpp"
#include "Core/CommonCore.hpp"
#include "Core/Graphics/Utility/TextureAtlas.hpp"
#include "Core/Graphics/Resource/GUIWidget.hpp"
#include "Core/Graphics/Resource/Font.hpp"
#include "Core/Graphics/Resource/Texture.hpp"
#include "Core/Graphics/Resource/Model.hpp"
#include <LinaGX/Core/InputMappings.hpp>

namespace Lina
{
	SystemInitializationInfo Lina_GetInitInfo()
	{
		LinaGX::MonitorInfo monitor = LinaGX::Window::GetPrimaryMonitorInfo();

		const uint32 w = monitor.size.x / 4;
		const uint32 h = static_cast<uint32>(static_cast<float>(w) * (static_cast<float>(monitor.size.y) / static_cast<float>(monitor.size.x)));

		LinaGX::VSyncStyle vsync;
		vsync.dx12Vsync	  = LinaGX::DXVsync::None;
		vsync.vulkanVsync = LinaGX::VKVsync::None;

		return SystemInitializationInfo{
			.appName	  = "Lina Editor",
			.windowWidth  = w,
			.windowHeight = h,
			.windowStyle  = LinaGX::WindowStyle::BorderlessApplication,
			.vsyncStyle	  = vsync,
			.allowTearing = true,
			.appDelegate  = new Lina::Editor::Editor(),
			.clearColor	  = Theme::GetDef().background0,
		};
	}
} // namespace Lina

namespace Lina::Editor
{
	Editor* Editor::s_editor = nullptr;

	bool Editor::PreInitialize()
	{
		ResourceList resources;
		if (!EditorResources::LoadPriorityResources(m_resourceManagerV2, resources))
		{
			LINA_ERR("Loading priority editor resources went bad, aborting!");
			return false;
		}

		m_editorRenderer.Initialize(this);
		m_editorRenderer.OnResourcesLoaded(resources);
		return true;
	}

	bool Editor::Initialize()
	{
		s_editor						 = this;
		Theme::GetDef().iconFont		 = EDITOR_FONT_ICON_ID;
		Theme::GetDef().defaultFont		 = EDITOR_FONT_ROBOTO_ID;
		Theme::GetDef().defaultBoldFont	 = EDITOR_FONT_ROBOTO_BOLD_ID;
		Theme::GetDef().altFont			 = EDITOR_FONT_PLAY_BOLD_ID;
		Theme::GetDef().altBigFont		 = EDITOR_FONT_PLAY_BIG_ID;
		Theme::GetDef().iconDropdown	 = ICON_ARROW_DOWN;
		Theme::GetDef().iconCircleFilled = ICON_CIRCLE_FILLED;
		Theme::GetDef().iconSliderHandle = ICON_CIRCLE_FILLED;

		m_atlasManager.Initialize(this);
		m_windowPanelManager.Initialize(this);

		m_mainWindow		   = m_app->GetApplicationWindow(LINA_MAIN_SWAPCHAIN);
		m_primaryWidgetManager = &m_windowPanelManager.GetSurfaceRenderer(LINA_MAIN_SWAPCHAIN)->GetWidgetManager();

		Widget* lock	 = m_windowPanelManager.LockAllForegrounds(m_mainWindow, Locale::GetStr(LocaleStr::WorkInProgressInAnotherWindow));
		Widget* prog	 = CommonWidgets::BuildGenericPopupProgress(lock, Locale::GetStr(LocaleStr::LoadingEngine), true);
		Text*	progText = static_cast<Text*>(prog->FindChildWithDebugName("Progress"));
		lock->AddChild(prog);

		Taskflow tf;

		TaskRunner::AddTask(
			[this, progText]() {
				progText->UpdateTextAndCalcSize(Locale::GetStr(LocaleStr::LoadingSettings));

				// Load editor settings or create and save empty if non-existing.
				const String userDataFolder = FileSystem::GetUserDataFolder() + "Editor/";
				const String settingsPath	= userDataFolder + "EditorSettings.linameta";
				m_settings.SetPath(settingsPath);
				if (!FileSystem::FileOrPathExists(userDataFolder))
					FileSystem::CreateFolderInPath(userDataFolder);
				if (FileSystem::FileOrPathExists(settingsPath))
				{
					if (!m_settings.LoadFromFile())
						m_settings.SaveToFile();
				}
				else
					m_settings.SaveToFile();

				progText->UpdateTextAndCalcSize(Locale::GetStr(LocaleStr::LoadingCoreResources));

				ResourceList coreResources;
				EditorResources::LoadCoreResources(m_resourceManagerV2, coreResources);

				progText->UpdateTextAndCalcSize(Locale::GetStr(LocaleStr::GeneratingAtlases));
				m_atlasManager.AddCustomAtlas("Resources/Editor/Textures/Atlas/ProjectIcons/", "ProjectIcons"_hs, Vector2ui(2048, 2048));
			},
			[this]() {
				Application::GetLGX()->Join();

				HashSet<Resource*> resources;
				resources.insert(m_resourceManagerV2.GetResource<Texture>(EDITOR_TEXTURE_CHECKERED_ID));
				resources.insert(m_resourceManagerV2.GetResource<Texture>(EDITOR_TEXTURE_PROTOTYPE_DARK_ID));
				resources.insert(m_resourceManagerV2.GetResource<Font>(EDITOR_FONT_PLAY_ID));
				resources.insert(m_resourceManagerV2.GetResource<Font>(EDITOR_FONT_PLAY_BOLD_ID));
				resources.insert(m_atlasManager.GetCustomAtlas("ProjectIcons"_hs)->GetRaw());
				m_editorRenderer.OnResourcesLoaded(resources);
				m_windowPanelManager.UnlockAllForegrounds();

				// Resize window to work dims.
				m_mainWindow->SetPosition(m_mainWindow->GetMonitorInfoFromWindow().workTopLeft);
				m_mainWindow->AddSizeRequest(m_mainWindow->GetMonitorWorkSize());

				// Insert editor root.
				Widget* root = m_primaryWidgetManager->GetRoot();
				m_editorRoot = root->GetWidgetManager()->Allocate<EditorRoot>("EditorRoot");
				m_editorRoot->Initialize();
				root->AddChild(m_editorRoot);

				// Launch project
				m_projectManager.Initialize(this);
				m_settings.GetLayout().ApplyStoredLayout();
			});

		return true;
	}

	void Editor::OnWindowSizeChanged(LinaGX::Window* window, const Vector2ui& size)
	{
		m_app->GetLGX()->Join();
		m_windowPanelManager.OnWindowSizeChanged(window, size);
	}
	void Editor::PreTick()
	{
		TaskRunner::Poll();
		m_projectManager.PreTick();
		m_windowPanelManager.PreTick();
		m_editorRenderer.PreTick();
	}

	void Editor::Tick(float delta)
	{
		m_editorRenderer.Tick(delta);
	}

	void Editor::Render()
	{
		m_editorRenderer.Render();
	}

	void Editor::PreShutdown()
	{
		m_editorRenderer.Shutdown();
		m_atlasManager.Shutdown();
		m_windowPanelManager.Shutdown();
		m_projectManager.Shutdown();
		m_resourceManagerV2.Shutdown();
	}

	void Editor::RequestExit()
	{
		SaveSettings();
		m_app->Quit();
	}

	void Editor::SaveSettings()
	{
		m_settings.GetLayout().StoreLayout();
		m_settings.SaveToFile();
	}

	void Editor::CreateWorldRenderer(EntityWorld* world)
	{
		WorldRenderer* wr = new WorldRenderer(world, m_app->GetApplicationWindow(LINA_MAIN_SWAPCHAIN)->GetSize());
		m_editorRenderer.AddWorldRenderer(wr);
		m_worldRenderers[world] = wr;
	}

	void Editor::DestroyWorldRenderer(EntityWorld* world)
	{
		auto it = m_worldRenderers.find(world);
		LINA_ASSERT(it != m_worldRenderers.end(), "");
		WorldRenderer* wr = it->second;
		m_editorRenderer.RemoveWorldRenderer(wr);
		delete wr;
		m_worldRenderers.erase(it);
	}

	WorldRenderer* Editor::GetWorldRenderer(EntityWorld* world)
	{
		return m_worldRenderers.at(world);
	}

} // namespace Lina::Editor
