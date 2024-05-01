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

#include "Common/System/Subsystem.hpp"

namespace Lina
{
	class GfxManager;
	class EntityWorld;
	class WorldRenderer;

	class WorldManager : public Subsystem
	{
	public:
		WorldManager(System* sys) : Subsystem(sys, SubsystemType::WorldManager){};
		virtual ~WorldManager() = default;

		virtual void Initialize(const SystemInitializationInfo& initInfo) override;
		virtual void Shutdown() override;

		void		   InstallWorld(const String& path);
		void		   ResizeWorldTexture(EntityWorld* world, const Vector2ui& newSize);
		WorldRenderer* GetWorldRenderer(EntityWorld* world);
		void		   SaveEmptyWorld(const String& absolutePath);
		void		   InstallLevel(const char* level);
		void		   UninstallLevel(bool immediate);
		void		   Tick(float deltaTime);

		inline EntityWorld* GetMainWorld() const
		{
			return m_mainWorld;
		}

	private:
		void UninstallMainWorld();

	private:
		GfxManager*			 m_gfxManager	= nullptr;
		EntityWorld*		 m_mainWorld	= nullptr;
		Vector<EntityWorld*> m_activeWorlds = {};
	};
} // namespace Lina