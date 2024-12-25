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

#include "Core/GUI/Widgets/Widget.hpp"
#include "Core/World/EntityWorld.hpp"

namespace Lina
{
	class WorldRenderer;
	class Font;
	class Text;
} // namespace Lina

namespace Lina::Editor
{
	class EditorCamera;
	class OrbitCamera;
	class EditorWorldRenderer;

	class WorldDisplayer : public Widget, public EntityWorldListener
	{
	public:
		enum class WorldCameraType
		{
			Orbit,
			FreeMove,
		};

		struct Properties
		{
			String noWorldText = "";
		};

		WorldDisplayer() : Widget(WF_CONTROLLABLE){};
		virtual ~WorldDisplayer() = default;

		virtual void Construct() override;
		virtual void Initialize() override;
		virtual void Destruct() override;
		virtual void PreTick() override;
		virtual void Tick(float dt) override;
		virtual void Draw() override;

		void DisplayWorld(WorldRenderer* renderer, EditorWorldRenderer* ewr, WorldCameraType cameraType);
		bool OnMouse(uint32 button, LinaGX::InputAction act) override;

		// World
		virtual void OnWorldTick(float delta, PlayMode playmode) override;

		inline EditorCamera* GetWorldCamera()
		{
			return m_camera;
		}

		inline Properties& GetProps()
		{
			return m_props;
		}

	private:
		void DestroyCamera();
		void DrawAxis(const Vector3& targetAxis, const Color& baseColor, const String& axis);

	private:
		EditorWorldRenderer* m_ewr			 = nullptr;
		Properties			 m_props		 = {};
		Text*				 m_noWorldText	 = nullptr;
		WorldRenderer*		 m_worldRenderer = nullptr;
		bool				 m_mouseConfined = false;
		EditorCamera*		 m_camera		 = nullptr;
		Font*				 m_gizmoFont	 = nullptr;
	};
	LINA_WIDGET_BEGIN(WorldDisplayer, Hidden)
	LINA_CLASS_END(WorldDisplayer)
} // namespace Lina::Editor
