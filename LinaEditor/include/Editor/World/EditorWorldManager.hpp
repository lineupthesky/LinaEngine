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

#include "Common/Math/Vector.hpp"

namespace Lina
{
	class EntityWorld;
	class WorldRenderer;
	class Entity;
	struct ResourceDirectory;
} // namespace Lina

namespace LinaGX
{
	class Window;
}
namespace Lina::Editor
{
	class Editor;
	class EditorWorldRenderer;

	class EditorWorldManagerListener
	{
	public:
		virtual void OnWorldManagerEntitySelectionChanged(EntityWorld* w, const Vector<Entity*>& entities, StringID source) {};
		virtual void OnWorldManagerEntityHierarchyChanged(EntityWorld* w) {};
		virtual void OnWorldManagerEntityPhysicsSettingsChanged(EntityWorld* w) {};
		virtual void OnWorldManagerEntityParamsChanged(EntityWorld* w) {};
		virtual void OnWorldManagerComponentsDataChanged(EntityWorld* w) {};
	};

	class EditorWorldManager
	{
	public:
		struct WorldData
		{
			EntityWorld*		 world				 = nullptr;
			EditorWorldRenderer* editorWorldRenderer = nullptr;
			Vector<Entity*>		 selectedEntities	 = {};
		};

	public:
		void Initialize(Editor* editor);
		void Shutdown();
		void Tick(float delta);

		void AddListener(EditorWorldManagerListener* listener);
		void RemoveListener(EditorWorldManagerListener* listener);

		EditorWorldRenderer* CreateEditorWorld(ResourceID space, LinaGX::Window* window);
		void				 DestroyEditorWorld(EntityWorld* world);

		EntityWorld* GetWorld(ResourceID id);
		WorldData&	 GetWorldData(EntityWorld* world);
		void		 BroadcastEntitySelectionChanged(EntityWorld* world, const Vector<Entity*>& selection, StringID source);
		void		 BroadcastEntityHierarchyChanged(EntityWorld* world);
		void		 BroadcastEntityPhysicsSettingsChanged(EntityWorld* world);
		void		 BroadcastEntityParamsChanged(EntityWorld* world);
		void		 BroadcastComponentsChanged(EntityWorld* world);

	private:
		JobExecutor							m_executor;
		Vector<WorldData>					m_worlds;
		Editor*								m_editor = nullptr;
		Vector<EditorWorldManagerListener*> m_listeners;
	};

} // namespace Lina::Editor
