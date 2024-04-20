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
#include "Core/World/EntityWorld.hpp"
#include "Core/GUI/Widgets/Compound/FileMenu.hpp"
#include "Editor/Editor.hpp"

namespace Lina
{
	class WorldManager;
	class Entity;
	class Selectable;
	class DirectionalLayout;
} // namespace Lina

namespace Lina::Editor
{
	class Editor;
	class PanelEntities : public Panel, public EntityWorldListener, public FileMenuListener, public EditorPayloadListener
	{
	public:
		PanelEntities() : Panel(PanelType::Entities, 0){};
		virtual ~PanelEntities() = default;

		virtual void Construct() override;
		virtual void Destruct() override;
		virtual void PreTick() override;
		virtual void Tick(float dt) override;
		virtual void Draw(int32 threadIndex) override;
		virtual bool OnFileMenuItemClicked(StringID sid, void* userData) override;
		virtual void OnGetFileMenuItems(StringID sid, Vector<FileMenuItem::Data>& outData, void* userData) override;

		virtual void			OnPayloadStarted(PayloadType type, Widget* payload) override;
		virtual void			OnPayloadEnded(PayloadType type, Widget* payload) override;
		virtual bool			OnPayloadDropped(PayloadType type, Widget* payload) override;
		virtual LinaGX::Window* OnPayloadGetWindow() override
		{
			return m_lgxWindow;
		}

		void RefreshHierarchy();

	private:
		void	CreateContextMenu(Widget* w);
		Widget* CreateEntitySelectable(Entity* e, uint8 level);

	private:
		Editor*				   m_editor				 = nullptr;
		WorldManager*		   m_worldManager		 = nullptr;
		EntityWorld*		   m_world				 = nullptr;
		DirectionalLayout*	   m_entitiesLayout		 = nullptr;
		bool				   m_entityPayloadActive = false;
		Vector<Selectable*>	   m_entitySelectables;
		HashMap<Entity*, bool> m_foldStatus;
	};

} // namespace Lina::Editor
