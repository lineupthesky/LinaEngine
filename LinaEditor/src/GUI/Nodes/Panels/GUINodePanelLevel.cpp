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

#include "GUI/Nodes/Panels/GUINodePanelLevel.hpp"
#include "GUI/Utility/GUIUtility.hpp"
#include "GUI/Nodes/Widgets/GUINodeText.hpp"
#include "GUI/Drawers/GUIDrawerBase.hpp"
#include "Graphics/Platform/LinaVGIncl.hpp"
#include "Graphics/Interfaces/IWindow.hpp"
#include "Graphics/Core/WorldRenderer.hpp"
#include "Graphics/Core/SurfaceRenderer.hpp"
#include "Core/Editor.hpp"
#include "Core/Theme.hpp"
#include "System/ISystem.hpp"
#include "World/Level/LevelManager.hpp"
#include "World/Level/Level.hpp"

// Debug
#include "Graphics/Core/WorldRenderer.hpp"
#include "World/Core/EntityWorld.hpp"
#include "Graphics/Components/CameraComponent.hpp"
#include "Graphics/Resource/Model.hpp"
#include "Resources/Core/ResourceManager.hpp"
#include "Math/Math.hpp"
#include "Core/SystemInfo.hpp"

namespace Lina::Editor
{
	Entity* camEntity = nullptr;

	GUINodePanelLevel::GUINodePanelLevel(GUIDrawerBase* drawer, int drawOrder, EditorPanel panelType, const String& title, GUINodeDockArea* parentDockArea) : GUINodePanel(drawer, drawOrder, panelType, title, parentDockArea)
	{
		m_levelManager	  = m_editor->GetSystem()->CastSubsystem<LevelManager>(SubsystemType::LevelManager);
		m_gfxManager	  = m_editor->GetSystem()->CastSubsystem<GfxManager>(SubsystemType::GfxManager);
		m_resourceManager = m_editor->GetSystem()->CastSubsystem<ResourceManager>(SubsystemType::ResourceManager);
		m_loadedLevel	  = m_levelManager->GetCurrentLevel();
		m_levelManager->GetSystem()->AddListener(this);

		m_noLevelText = new GUINodeText(drawer, drawOrder);
		m_noLevelText->SetAlignment(TextAlignment::Center);
		m_noLevelText->SetFont(FontType::BigEditor);
		m_noLevelText->SetVisible(m_loadedLevel != nullptr);
		m_noLevelText->SetText("NO LEVEL INSTALLED");
		m_noLevelText->SetBothColors(Theme::TC_SilentTransparent);

		m_noLevelTextAlt = new GUINodeTextRichColors(drawer, drawOrder);
		m_noLevelTextAlt->SetAlignment(TextAlignment::Center);
		m_noLevelTextAlt->SetFont(FontType::BigEditor);
		m_noLevelTextAlt->SetScale(0.5f);
		m_noLevelTextAlt->SetVisible(m_loadedLevel != nullptr);
		m_noLevelTextAlt->AddText("Go to ", Theme::TC_Dark1);
		m_noLevelTextAlt->AddText("Level > New Level ", Theme::TC_SilentTransparent);
		m_noLevelTextAlt->AddText("to add one.", Theme::TC_Dark2);

		AddChildren(m_noLevelText);
		AddChildren(m_noLevelTextAlt);
	}

	GUINodePanelLevel::~GUINodePanelLevel()
	{
		if (m_worldRenderer)
		{
			m_drawer->GetSurfaceRenderer()->DeleteWorldRenderer(m_worldRenderer);
			LINA_WARN("DELETED WORLD RENDERER");
		}
		m_levelManager->GetSystem()->RemoveListener(this);
	}

	void GUINodePanelLevel::Draw(int threadID)
	{
		GUINodePanel::Draw(threadID);

		const float padding = Theme::GetProperty(ThemeProperty::GeneralItemPadding, m_window->GetDPIScale());

		// Handle no level.
		{
			if (!m_loadedLevel)
			{
				m_noLevelText->SetPos(m_rect.GetCenter());
				m_noLevelTextAlt->SetPos(m_rect.GetCenter() + Vector2(0, padding * 2));
				m_noLevelText->Draw(threadID);
				m_noLevelTextAlt->Draw(threadID);
				return;
			}
		}

		if (m_loadedLevel && !m_worldRenderer)
		{
			LINA_WARN("CREATED WORLD RENDERER");
			m_drawer->GetSurfaceRenderer()->CreateWorldRenderer(BIND(&GUINodePanelLevel::OnWorldRendererCreated, this, std::placeholders::_1), m_loadedLevel->GetWorld(), m_rect.size, WRM_None);
		}

		if (m_worldRenderer)
		{
			auto e = m_loadedLevel->GetWorld()->GetEntity("Cube");

			if (e)
			{
				float prevX = e->GetPosition().x;
				e->AddPosition(Vector3(SystemInfo::GetDeltaTimeF() * 0.60f, 0, 0));

				if (e->GetPosition().x > 3.5f)
					e->SetPosition(Vector3(-3.5f, 0.0f, 0.0f));
			}

			const StringID textureHandle = m_worldRenderer->GetFinalTexture()->GetSID();
			LinaVG::DrawImage(threadID, textureHandle, LV2(m_rect.GetCenter()), LV2(m_rect.size));

			if (!m_rect.size.Equals(m_sizeWhenWorldRendererCreated, 0.01f))
			{
				m_sizeWhenWorldRendererCreated = m_rect.size;
				m_worldRenderer->AddResizeRequest(m_rect.size, m_rect.size.x / m_rect.size.y);
			}
		}
	}

	void GUINodePanelLevel::OnSystemEvent(SystemEvent eventType, const Event& ev)
	{
		if (eventType & EVS_LevelInstalled)
		{
			m_loadedLevel = static_cast<Level*>(ev.pParams[0]);
		}
		else if (eventType & EVS_LevelUninstalled)
			m_loadedLevel = nullptr;

		m_noLevelText->SetVisible(m_loadedLevel != nullptr);
	}
	void GUINodePanelLevel::OnWorldRendererCreated(WorldRenderer* renderer)
	{
		m_worldRenderer = renderer;

		camEntity = m_loadedLevel->GetWorld()->CreateEntity("Cam Entity");
		auto cam  = m_loadedLevel->GetWorld()->AddComponent<CameraComponent>(camEntity);
		camEntity->SetPosition(Vector3(0, 0, -5));
		camEntity->SetRotationAngles(Vector3(0, 0, 0));

		m_loadedLevel->GetWorld()->SetActiveCamera(cam);
		auto aq = m_resourceManager->GetResource<Model>("Resources/Core/Models/Cube.fbx"_hs)->AddToWorld(m_loadedLevel->GetWorld());
		aq->SetName("Cube");
		//	auto aq2 = m_resourceManager->GetResource<Model>("ContentBrowser/Core/Models/Cube.fbx"_hs)->AddToWorld(testWorld);
		/// auto aq3 = m_resourceManager->GetResource<Model>("ContentBrowser/Core/Models/Capsule.fbx"_hs)->AddToWorld(testWorld);
		aq->SetPosition(Vector3(-3.5f, 0, 0));
		//	aq2->SetPosition(Vector3(3, 0, 0));
		// aq3->SetPosition(Vector3(0, 0, 0));
		m_sizeWhenWorldRendererCreated = m_rect.size;
	}
} // namespace Lina::Editor
