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
#include "Common/Math/Rect.hpp"
#include "Core/GUI/Widgets/WidgetManager.hpp"
#include "Core/GUI/CommonGUI.hpp"
#include "Core/GUI/Theme.hpp"
#include "Common/Data/Bitmask.hpp"

namespace LinaGX
{
	class Window;

	enum class InputAction;

} // namespace LinaGX

namespace LinaVG
{
	struct StyleOptions;
}

namespace Lina
{
	class System;
	class ResourceManager;
	class IStream;
	class Ostream;

#define V2_GET_MUTATE(NAME, VAR)                                                                                                                                                                                                                                   \
	inline void Set##NAME(const Vector2& sz)                                                                                                                                                                                                                       \
	{                                                                                                                                                                                                                                                              \
		VAR = sz;                                                                                                                                                                                                                                                  \
	}                                                                                                                                                                                                                                                              \
	inline void Set##NAME##X(float x)                                                                                                                                                                                                                              \
	{                                                                                                                                                                                                                                                              \
		VAR.x = x;                                                                                                                                                                                                                                                 \
	}                                                                                                                                                                                                                                                              \
	inline void Set##NAME##Y(float y)                                                                                                                                                                                                                              \
	{                                                                                                                                                                                                                                                              \
		VAR.y = y;                                                                                                                                                                                                                                                 \
	}                                                                                                                                                                                                                                                              \
	inline const Vector2& Get##NAME() const                                                                                                                                                                                                                        \
	{                                                                                                                                                                                                                                                              \
		return VAR;                                                                                                                                                                                                                                                \
	}                                                                                                                                                                                                                                                              \
	inline float Get##NAME##X() const                                                                                                                                                                                                                              \
	{                                                                                                                                                                                                                                                              \
		return VAR.x;                                                                                                                                                                                                                                              \
	}                                                                                                                                                                                                                                                              \
	inline float Get##NAME##Y() const                                                                                                                                                                                                                              \
	{                                                                                                                                                                                                                                                              \
		return VAR.y;                                                                                                                                                                                                                                              \
	}

#define V2_INCREMENTERS(NAME, VAR)                                                                                                                                                                                                                                 \
	inline void Add##NAME##X(float x)                                                                                                                                                                                                                              \
	{                                                                                                                                                                                                                                                              \
		VAR.x += x;                                                                                                                                                                                                                                                \
	}                                                                                                                                                                                                                                                              \
	inline void Add##NAME##Y(float y)                                                                                                                                                                                                                              \
	{                                                                                                                                                                                                                                                              \
		VAR.y += y;                                                                                                                                                                                                                                                \
	}

	class Widget
	{
	public:
		virtual void PreTick(){};
		virtual void CalculateSize(float delta){};
		virtual void Tick(float delta){};
		virtual void Draw(int32 threadIndex);

		void		 AddChild(Widget* w);
		void		 RemoveChild(Widget* w);
		void		 RemoveAllChildren();
		virtual void SaveToStream(OStream& out) const;
		virtual void LoadFromStream(IStream& in);
		void		 SetIsHovered();
		void		 DrawBorders(int32 threadIndex);
		void		 DrawTooltip(int32 threadIndex);
		Vector2		 GetStartFromMargins();
		Vector2		 GetEndFromMargins();
		void		 SetIsDisabled(bool isDisabled);
		Rect		 GetTemporaryAlignedRect();

		Vector2 GetWindowSize();
		Vector2 GetMonitorSize();
		Vector2 GetWindowPos();

		virtual void Construct(){};
		virtual void Destruct(){};
		virtual void Initialize();
		virtual bool OnMouse(uint32 button, LinaGX::InputAction action);
		virtual bool OnKey(uint32 keycode, int32 scancode, LinaGX::InputAction action);
		virtual void DebugDraw(int32 threadIndex, int32 drawOrder);

		Widget* FindGetControlsManager();
		void	GrabControls(Widget* widget);
		void	ReleaseControls(Widget* widget);
		bool	CanGrabControls(Widget* widget);
		Widget* GetControlsOwner();
		void	MoveControlsToNext();
		void	MoveControlsToPrev();
		bool	IsWidgetInHierarchy(Widget* widget);

		inline Widget* GetCustomTooltip()
		{
			return m_customTooltip;
		}

		virtual LinaGX::CursorType GetCursorOverride()
		{
			return LinaGX::CursorType::Default;
		}

		template <typename... Args> void AddChild(Args&&... args)
		{
			(AddChild(std::forward<Widget*>(args)), ...);
		}

		inline const Rect& GetRect()
		{
			return m_rect;
		}

		inline float GetHalfSizeX() const
		{
			return m_rect.size.x * 0.5f;
		}

		inline float GetHalfSizeY() const
		{
			return m_rect.size.y * 0.5f;
		}

		inline const Vector2 GetHalfSize() const
		{
			return m_rect.size * 0.5f;
		}

		inline LinaGX::Window* GetWindow()
		{
			return m_lgxWindow;
		}

		inline int32 GetDrawOrder() const
		{
			return m_drawOrder;
		}

		inline void SetDrawOrder(int32 order)
		{
			m_drawOrder = order;
		}

		inline bool GetIsHovered() const
		{
			return m_isHovered;
		}

		inline void SetDebugName(const String& dbgName)
		{
			m_debugName = dbgName;
		}

		inline const String& GetDebugName() const
		{
			return m_debugName;
		}

		inline void SetChildID(uint32 idx)
		{
			m_childID = idx;
		}

		inline const Vector<Widget*>& GetChildren() const
		{
			return m_children;
		}

		inline Bitmask32& GetFlags()
		{
			return m_flags;
		}

		inline Widget* GetParent()
		{
			return m_parent;
		}

		inline TypeID GetTID() const
		{
			return m_tid;
		}

		inline bool GetIsPressed() const
		{
			return m_isPressed;
		}

		inline TBLR& GetBorderThickness()
		{
			return m_borderThickness;
		}

		inline void SetBorderColor(const Color& c)
		{
			m_colorBorders = c;
		}

		inline Color& GetBorderColor()
		{
			return m_colorBorders;
		}

		inline const String& GetTooltip() const
		{
			return m_tooltip;
		}

		inline void SetTooltip(const String& str)
		{
			m_tooltip = str;
		}

		inline void SetPosAlignmentSourceX(PosAlignmentSource src)
		{
			m_posAlignSourceX = src;
		}

		inline void SetPosAlignmentSourceY(PosAlignmentSource src)
		{
			m_posAlignSourceY = src;
		}

		inline PosAlignmentSource GetPosAlignmentSourceX()
		{
			return m_posAlignSourceX;
		}

		inline PosAlignmentSource GetPosAlignmentSourceY()
		{
			return m_posAlignSourceY;
		}

		inline TBLR& GetChildMargins()
		{
			return m_childMargins;
		}

		inline float GetChildPadding() const
		{
			return m_childPadding;
		}

		inline void SetChildPadding(float padding)
		{
			m_childPadding = padding;
		}

		inline bool GetIsDisabled() const
		{
			return m_isDisabled;
		}

		inline WidgetManager* GetWidgetManager() const
		{
			return m_manager;
		}

		inline System* GetSystem() const
		{
			return m_system;
		}

		template <typename T> static void GetWidgetsOfType(Vector<T*>& outWidgets, Widget* root)
		{
			auto tid = GetTypeID<T>();

			for (auto* c : root->GetChildren())
			{
				if (c->GetTID() == tid)
					outWidgets.push_back(static_cast<T*>(c));

				GetWidgetsOfType<T>(outWidgets, c);
			}
		}

		inline void SetVisible(bool visible)
		{
			m_isVisible = visible;
		}

		inline bool GetIsVisible() const
		{
			return m_isVisible;
		}

		inline void SetBuildCustomTooltip(Delegate<Widget*()>&& tt)
		{
			m_buildCustomTooltip = tt;
		}

		V2_GET_MUTATE(FixedSize, m_fixedSize);
		V2_GET_MUTATE(AlignedSize, m_alignedSize);
		V2_INCREMENTERS(AlignedSize, m_alignedSize);
		V2_GET_MUTATE(Pos, m_rect.pos);
		V2_GET_MUTATE(Size, m_rect.size);
		V2_GET_MUTATE(AlignedPos, m_alignedPos);
		V2_INCREMENTERS(AlignedPos, m_alignedPos);

	protected:
		friend class WidgetManager;

		Widget(int32 maxChilds = -1, Bitmask32 flags = 0) : m_maxChilds(maxChilds), m_flags(flags){};
		virtual ~Widget() = default;

		Widget* FindNextSelectable(Widget* start);
		Widget* FindPreviousSelectable(Widget* start);

	protected:
		TypeID				m_tid		= 0;
		WidgetManager*		m_manager	= nullptr;
		Widget*				m_parent	= nullptr;
		LinaGX::Window*		m_lgxWindow = nullptr;
		int32				m_drawOrder = 0;
		System*				m_system	= nullptr;
		Rect				m_rect		= {};
		Vector<Widget*>		m_children;
		int32				m_maxChilds		  = 0;
		bool				m_isHovered		  = false;
		bool				m_isPressed		  = false;
		ResourceManager*	m_resourceManager = nullptr;
		String				m_debugName		  = "Widget";
		uint32				m_childID		  = 0;
		Bitmask32			m_flags			  = 0;
		Widget*				m_next			  = nullptr;
		Widget*				m_prev			  = nullptr;
		Vector2				m_alignedSize	  = Vector2::Zero;
		Vector2				m_alignedPos	  = Vector2::Zero;
		TBLR				m_childMargins	  = {};
		Vector2				m_fixedSize		  = Vector2::Zero;
		Color				m_colorBorders	  = Theme::GetDef().background2;
		TBLR				m_borderThickness = {};
		String				m_tooltip		  = "";
		PosAlignmentSource	m_posAlignSourceX = PosAlignmentSource::Start;
		PosAlignmentSource	m_posAlignSourceY = PosAlignmentSource::Start;
		float				m_childPadding	  = 0.0f;
		bool				m_isDisabled	  = false;
		Widget*				m_controlsOwner	  = nullptr;
		Widget*				m_controlsManager = nullptr;
		bool				m_isVisible		  = true;
		Delegate<Widget*()> m_buildCustomTooltip;
		Widget*				m_customTooltip = nullptr;
	};

} // namespace Lina
