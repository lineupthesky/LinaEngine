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

#include "Editor/Widgets/Panel/Panel.hpp"
#include "Editor/PayloadListener.hpp"
#include "Core/World/CommonWorld.hpp"

namespace Lina
{
	class EntityWorld;
	class WorldRenderer;
	class Entity;
} // namespace Lina

namespace Lina::Editor
{
	class Editor;
	class WorldDisplayer;
	class GridRenderer;
	class GizmoRenderer;
	class MousePickRenderer;

	class PanelWorld : public Panel, public EditorPayloadListener
	{
	public:
		PanelWorld() : Panel(PanelType::World){};
		virtual ~PanelWorld() = default;

		virtual void Construct() override;
		virtual void Destruct() override;
		virtual void Tick(float delta) override;

		void CreateWorld(const String& resourcePath);
		void DestroyWorld();

		void SelectEntity(Entity* e, bool clearOthers);
		void SelectEntity(EntityID guid, bool clearOthers);
		void ChangeSelectionByAction(const Vector<EntityID>& selection);
		void OnEntitySelectionChanged();

		virtual void OnPayloadStarted(PayloadType type, Widget* payload) override;
		virtual void OnPayloadEnded(PayloadType type, Widget* payload) override;
		virtual bool OnPayloadDropped(PayloadType type, Widget* payload) override;

		virtual bool OnMouse(uint32 button, LinaGX::InputAction act) override;

		virtual LinaGX::Window* OnPayloadGetWindow()
		{
			return m_lgxWindow;
		}

		inline EntityWorld* GetWorld() const
		{
			return m_world;
		}

	private:
		Vector<Entity*>	   m_selectedEntities  = {};
		Editor*			   m_editor			   = nullptr;
		EntityWorld*	   m_world			   = nullptr;
		WorldRenderer*	   m_worldRenderer	   = nullptr;
		GridRenderer*	   m_gridRenderer	   = nullptr;
		WorldDisplayer*	   m_worldDisplayer	   = nullptr;
		GizmoRenderer*	   m_gizmoRenderer	   = nullptr;
		MousePickRenderer* m_mousePickRenderer = nullptr;
	};

	LINA_WIDGET_BEGIN(PanelWorld, Hidden)
	LINA_CLASS_END(PanelWorld)

} // namespace Lina::Editor
