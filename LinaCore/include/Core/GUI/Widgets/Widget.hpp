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
#include "Common/Memory/CommonMemory.hpp"
#include "Core/GUI/CommonGUI.hpp"
#include "Core/GUI/Theme.hpp"
#include "Common/Data/Bitmask.hpp"
#include "Core/Reflection/WidgetReflection.hpp"

namespace LinaGX
{
	class Window;

	enum class InputAction;

} // namespace LinaGX

namespace LinaVG
{
	struct StyleOptions;
	class Drawer;
} // namespace LinaVG

namespace Lina
{
	class System;
	class ResourceManagerV2;
	class IStream;
	class Ostream;
	class WidgetManager;
	struct TextureAtlasImage;
	class Texture;

#define WIDGET_VERSION		0
#define WIDGETPROPS_VERSION 0
#define DS_VERSION			0
	/*
		0: Initial
	 */

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

	struct DropshadowProps
	{
		bool		enabled			   = false;
		bool		isInner			   = false;
		float		margin			   = 0.0f;
		float		thickness		   = Theme::GetDef().baseOutlineThickness;
		float		rounding		   = 0.0f;
		int32		drawOrderIncrement = 0;
		uint32		steps			   = 4;
		Color		color			   = Theme::GetDef().black;
		Direction	direction		   = Direction::Center;
		Vector<int> onlyRound;

		virtual void SaveToStream(OStream& stream) const;
		virtual void LoadFromStream(IStream& stream);
	};

	struct WidgetProps
	{
		bool				 clipChildren				 = false;
		bool				 customClip					 = false;
		bool				 drawBackground				 = false;
		bool				 backgroundIsCentralGradient = false;
		bool				 interpolateColor			 = false;
		bool				 fitTexture					 = false;
		bool				 hoveredIsDifferentColor	 = false;
		bool				 pressedIsDifferentColor	 = false;
		bool				 activeTextureTiling		 = false;
		bool				 useSpecialTexture			 = false;
		bool				 outlineIsInner				 = false;
		float				 colorInterpolateSpeed		 = 0.0f;
		float				 outlineThickness			 = Theme::GetDef().baseOutlineThickness;
		float				 rounding					 = 0.0f;
		float				 childPadding				 = 0.0f;
		void*				 lvgUserData				 = nullptr;
		int32				 drawOrderIncrement			 = 0;
		String				 tooltip					 = "";
		String				 debugName					 = "";
		TBLR				 childMargins				 = {};
		TBLR				 borderThickness			 = {};
		Color				 colorBorders				 = Theme::GetDef().outlineColorBase;
		ColorGrad			 colorBackground			 = Theme::GetDef().background0;
		ColorGrad			 colorOutline				 = Theme::GetDef().outlineColorBase;
		ColorGrad			 colorOutlineControls		 = Theme::GetDef().outlineColorControls;
		ColorGrad			 colorHovered				 = Theme::GetDef().background2;
		ColorGrad			 colorPressed				 = Theme::GetDef().background1;
		ColorGrad			 colorDisabled				 = Theme::GetDef().silent0;
		DirectionOrientation colorBackgroundDirection	 = DirectionOrientation::Horizontal;
		TextureAtlasImage*	 textureAtlas				 = nullptr;
		Texture*			 rawTexture					 = nullptr;
		void*				 specialTexture				 = 0;
		Vector2				 textureTiling				 = Vector2::One;
		Vector<int32>		 onlyRound					 = {};
		Rect				 customClipRect				 = {};
		ColorGrad			 _interpolatedColor			 = Color();
		DropshadowProps		 dropshadow;

		virtual void SaveToStream(OStream& stream) const;
		virtual void LoadFromStream(IStream& stream);
	};

	/*
	struct TransformProps
	{
		Rect rect = Rect();
		Vector2 alignedSize = Vector2::Zero;
		Vector2 alignedPos	= Vector2::Zero;
		Vector2 fixedSize	= Vector2::Zero;
		Anchor	anchorX		= Anchor::Start;
		Anchor	anchorY		= Anchor::Start;
	};
	*/
	class Widget
	{
	public:
		Widget(Bitmask32 flags = 0) : m_flags(flags), m_tid(GetTypeID<Widget>()){};
		virtual ~Widget() = default;

		virtual void Initialize();
		virtual void Construct(){};
		virtual void Destruct(){};
		virtual void PreDestruct(){};
		virtual bool OnMouse(uint32 button, LinaGX::InputAction action)
		{
			return false;
		};
		virtual bool OnMouseWheel(float amt)
		{
			return false;
		};
		virtual bool OnMousePos(const Vector2& p)
		{
			return false;
		};
		virtual bool OnKey(uint32 keycode, int32 scancode, LinaGX::InputAction action)
		{
			return false;
		};

		virtual Widget* GetNextControls()
		{
			return nullptr;
		}

		virtual Widget* GetPrevControls()
		{
			return nullptr;
		}

		virtual void OnGrabbedControls(bool isForward, Widget* prevControls){

		};

		virtual void DebugDraw(int32 drawOrder);
		virtual void PreTick(){};
		virtual void CalculateSize(float delta){};
		virtual void Tick(float delta){};
		virtual void Draw();
		virtual void SaveToStream(OStream& stream) const;
		virtual void LoadFromStream(IStream& stream);

		void	SetWidgetManager(WidgetManager* wm);
		void	DrawDropshadow();
		void	DrawBackground();
		void	DrawChildren();
		void	AddChild(Widget* w);
		void	ExecuteNextFrame(Delegate<void()>&& cb);
		void	RemoveChild(Widget* w);
		void	DeallocAllChildren();
		void	RemoveAllChildren();
		void	SetIsHovered();
		void	SetIsDisabled(bool isDisabled);
		void	DrawBorders();
		void	DrawTooltip();
		Vector2 GetStartFromMargins();
		Vector2 GetEndFromMargins();
		Rect	GetTemporaryAlignedRect();
		Vector2 GetWindowSize();
		Vector2 GetMonitorSize();
		Vector2 GetWindowPos();
		Widget* FindChildWithUserdata(void* ud);
		Widget* FindChildWithDebugName(const String& name);
		Widget* FindDeepestHovered();

		bool IsWidgetInHierarchy(Widget* widget);

		virtual float CalculateChildrenSize()
		{
			return 0.0f;
		}

		inline Widget* GetCustomTooltip()
		{
			return m_customTooltip;
		}

		virtual LinaGX::CursorType GetCursorOverride()
		{
			return LinaGX::CursorType::Default;
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

		inline TypeID SetTID(TypeID tid)
		{
			m_tid = tid;
		}

		inline TypeID GetTID() const
		{
			return m_tid;
		}

		inline bool GetIsPressed() const
		{
			return m_isPressed;
		}

		inline void SetAnchorX(Anchor src)
		{
			m_anchorX = src;
		}

		inline void SetAnchorY(Anchor src)
		{
			m_anchorY = src;
		}

		inline Anchor GetPosAlignmentSourceX()
		{
			return m_anchorX;
		}

		inline Anchor GetPosAlignmentSourceY()
		{
			return m_anchorY;
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

		inline void SetVisible(bool visible)
		{
			m_isVisible = visible;
		}

		inline bool GetIsVisible() const
		{
			return m_isVisible;
		}

		inline void SetCustomTooltipUserData(void* ud)
		{
			m_customTooltipUserData = ud;
		}

		inline void SetBuildCustomTooltip(Delegate<Widget*(void*)>&& tt)
		{
			m_buildCustomTooltip = tt;
		}

		inline void SetScrollerOffset(float f)
		{
			m_scrollerOffset = f;
		}

		inline float GetScrollerOffset() const
		{
			return m_scrollerOffset;
		}

		inline void SetLocalControlsManager(Widget* w)
		{
			m_localControlsManager = w;
		}

		inline Widget* GetLocalControlsManager() const
		{
			return m_localControlsManager;
		}

		inline void SetLocalControlsOwner(Widget* w)
		{
			m_localControlsOwner = w;
		}

		inline Widget* GetLocalControlsOwner() const
		{
			return m_localControlsOwner;
		}

		inline void SetUserData(void* data)
		{
			m_userData = data;
		}

		inline void* GetUserData() const
		{
			return m_userData;
		}

		inline void SetTickHook(Delegate<void(float delta)>&& hook)
		{
			m_tickHook = hook;
		}

		inline void AddPreTickHook(Delegate<void()>&& hook)
		{
			m_preTickHooks.push_back(std::move(hook));
		}

		template <typename... Args> void AddChild(Args&&... args)
		{
			(AddChild(std::forward<Widget*>(args)), ...);
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

		template <typename T> static T* GetWidgetOfType(Widget* root)
		{
			auto tid = GetTypeID<T>();

			for (Widget* c : root->GetChildren())
			{
				if (c->GetTID() == tid)
					return static_cast<T*>(c);

				T* widgetInChild = GetWidgetOfType<T>(c);
				if (widgetInChild != nullptr)
					return widgetInChild;
			}

			return nullptr;
		}

		inline WidgetProps& GetWidgetProps()
		{
			return m_widgetProps;
		}

		inline uint32 GetCacheIndex() const
		{
			return m_cacheIndex;
		}

		inline void SetDestructHook(Delegate<void()> hook)
		{
			m_destructHook = hook;
		}

		inline void AddChildRequest(Widget* w)
		{
			m_addChildRequests.push_back(w);
		}

		V2_GET_MUTATE(FixedSize, m_fixedSize);
		V2_GET_MUTATE(AlignedSize, m_alignedSize);
		V2_INCREMENTERS(AlignedSize, m_alignedSize);
		V2_GET_MUTATE(Pos, m_rect.pos);
		V2_GET_MUTATE(Size, m_rect.size);
		V2_GET_MUTATE(AlignedPos, m_alignedPos);
		V2_INCREMENTERS(AlignedPos, m_alignedPos);
		LinaVG::Drawer* m_lvg = nullptr;

		bool _fold = false;

	protected:
		friend class WidgetManager;
		template <typename U> friend class WidgetCache;

		void ChangedParent(Widget* w);
		void CheckCustomTooltip();

	protected:
		LINA_REFLECTION_ACCESS(Widget);
		ALLOCATOR_BUCKET_MEM;
		WidgetManager*				m_manager			   = nullptr;
		ResourceManagerV2*			m_resourceManager	   = nullptr;
		LinaGX::Window*				m_lgxWindow			   = nullptr;
		System*						m_system			   = nullptr;
		Widget*						m_parent			   = nullptr;
		Widget*						m_next				   = nullptr;
		Widget*						m_prev				   = nullptr;
		Widget*						m_customTooltip		   = nullptr;
		Widget*						m_localControlsManager = nullptr;
		Widget*						m_localControlsOwner   = nullptr;
		Delegate<Widget*(void*)>	m_buildCustomTooltip;
		Vector<Widget*>				m_children;
		Vector<Delegate<void()>>	m_executeNextFrame;
		Delegate<void()>			m_destructHook;
		Delegate<void(float delta)> m_tickHook;
		Vector<Delegate<void()>>	m_preTickHooks;
		Rect						m_rect					= {};
		Vector2						m_fixedSize				= Vector2::Zero;
		Vector2						m_alignedPos			= Vector2::Zero;
		Vector2						m_alignedSize			= Vector2::Zero;
		TypeID						m_tid					= 0;
		int32						m_drawOrder				= 0;
		Bitmask32					m_flags					= 0;
		Anchor						m_anchorX				= Anchor::Start;
		Anchor						m_anchorY				= Anchor::Start;
		bool						m_isHovered				= false;
		bool						m_isPressed				= false;
		bool						m_isDisabled			= false;
		bool						m_isVisible				= true;
		uint32						m_loadedVersion			= 0;
		float						m_scrollerOffset		= 0.0f;
		void*						m_customTooltipUserData = nullptr;
		void*						m_userData				= nullptr;
		uint32						m_cacheIndex			= 0;
		WidgetProps					m_widgetProps			= {};
		bool						m_initializing			= false;
		Vector<Widget*>				m_addChildRequests;
	};

	LINA_WIDGET_BEGIN(Widget, General)
	LINA_FIELD(Widget, m_flags, "Flags", FieldType::Bitmask32, GetTypeID<WidgetFlags>())
	LINA_FIELD(Widget, m_alignedPos, "Aligned Position", FieldType::Vector2, 0)
	LINA_FIELD_LIMITS(Widget, m_alignedPos, 0.0f, 1.0f, 0.1f)
	LINA_FIELD(Widget, m_alignedSize, "Aligned Size", FieldType::Vector2, 0)
	LINA_FIELD_LIMITS(Widget, m_alignedSize, 0.0f, 1.0f, 0.1f)
	LINA_FIELD(Widget, m_fixedSize, "Fixed Size", FieldType::Vector2, 0)
	LINA_FIELD(Widget, m_anchorX, "Anchor X", FieldType::Enum, GetTypeID<Anchor>())
	LINA_FIELD(Widget, m_anchorY, "Anchor Y", FieldType::Enum, GetTypeID<Anchor>())
	LINA_FIELD(Widget, m_widgetProps, "Widget Properties", FieldType::UserClass, GetTypeID<WidgetProps>())
	LINA_CLASS_END(Widget)

	LINA_CLASS_BEGIN(DropshadowProps)
	LINA_FIELD(DropshadowProps, enabled, "Enabled", FieldType::Boolean, 0);
	LINA_FIELD(DropshadowProps, isInner, "Is Inner", FieldType::Boolean, 0);
	LINA_FIELD(DropshadowProps, margin, "Margin", FieldType::Float, 0);
	LINA_FIELD(DropshadowProps, thickness, "Thickness", FieldType::Float, 0);
	LINA_FIELD(DropshadowProps, rounding, "Rounding", FieldType::Float, 0);
	LINA_FIELD(DropshadowProps, drawOrderIncrement, "Draw Order Increment", FieldType::Int32, 0);
	LINA_FIELD(DropshadowProps, steps, "Steps", FieldType::UInt32, 0);
	LINA_FIELD(DropshadowProps, color, "Color", FieldType::Color, 0);
	LINA_FIELD(DropshadowProps, direction, "Direction", FieldType::Enum, GetTypeID<Direction>());
	LINA_FIELD_VEC(DropshadowProps, onlyRound, "Only Round", FieldType::Int32, int32, 0);

	LINA_FIELD_DEPENDENCY_POS(DropshadowProps, isInner, "enabled", 1);
	LINA_FIELD_DEPENDENCY_POS(DropshadowProps, margin, "enabled", 1);
	LINA_FIELD_DEPENDENCY_POS(DropshadowProps, thickness, "enabled", 1);
	LINA_FIELD_DEPENDENCY_POS(DropshadowProps, rounding, "enabled", 1);
	LINA_FIELD_DEPENDENCY_POS(DropshadowProps, drawOrderIncrement, "enabled", 1);
	LINA_FIELD_DEPENDENCY_POS(DropshadowProps, steps, "enabled", 1);
	LINA_FIELD_DEPENDENCY_POS(DropshadowProps, color, "enabled", 1);
	LINA_FIELD_DEPENDENCY_POS(DropshadowProps, direction, "enabled", 1);
	LINA_FIELD_DEPENDENCY_POS(DropshadowProps, onlyRound, "enabled", 1);
	LINA_CLASS_END(DropshadowProps)

	LINA_CLASS_BEGIN(WidgetProps)
	LINA_FIELD(WidgetProps, debugName, "Debug Name", FieldType::String, 0);
	LINA_FIELD(WidgetProps, tooltip, "Tooltip", FieldType::String, 0);
	LINA_FIELD(WidgetProps, drawOrderIncrement, "Draw Order Increment", FieldType::Int32, 0);
	LINA_FIELD(WidgetProps, clipChildren, "Clip Children", FieldType::Boolean, 0);
	LINA_FIELD(WidgetProps, drawBackground, "Draw Background", FieldType::Boolean, 0);
	LINA_FIELD(WidgetProps, backgroundIsCentralGradient, "Central Gradient", FieldType::Boolean, 0);
	LINA_FIELD(WidgetProps, interpolateColor, "Interpolate Color", FieldType::Boolean, 0);
	LINA_FIELD(WidgetProps, colorInterpolateSpeed, "Color Interpolate Speed", FieldType::Float, 0);
	LINA_FIELD(WidgetProps, colorBackgroundDirection, "Background Direction", FieldType::Enum, GetTypeID<DirectionOrientation>());
	LINA_FIELD(WidgetProps, colorBackground, "Background Color", FieldType::ColorGrad, 0);
	LINA_FIELD(WidgetProps, colorOutline, "Outline Color", FieldType::ColorGrad, 0);
	LINA_FIELD(WidgetProps, colorOutlineControls, "Outline Color in Control", FieldType::ColorGrad, 0);
	LINA_FIELD(WidgetProps, colorDisabled, "Disabled Color", FieldType::ColorGrad, 0);
	LINA_FIELD(WidgetProps, hoveredIsDifferentColor, "Use Hovered Color", FieldType::Boolean, 0);
	LINA_FIELD(WidgetProps, colorHovered, "Hovered Color", FieldType::ColorGrad, 0);
	LINA_FIELD(WidgetProps, pressedIsDifferentColor, "Use Pressed Color", FieldType::Boolean, 0);
	LINA_FIELD(WidgetProps, colorPressed, "Pressed Color", FieldType::ColorGrad, 0);
	LINA_FIELD(WidgetProps, outlineThickness, "Outline Thickness", FieldType::Float, 0);
	LINA_FIELD(WidgetProps, outlineIsInner, "Inner Outline", FieldType::Boolean, 0);
	LINA_FIELD(WidgetProps, rounding, "Rounding", FieldType::Float, 0);
	LINA_FIELD(WidgetProps, fitTexture, "Fit Texture", FieldType::Boolean, 0);
	LINA_FIELD(WidgetProps, textureTiling, "Texture Tiling", FieldType::Vector2, 0);
	LINA_FIELD(WidgetProps, activeTextureTiling, "Active Texture Tiling", FieldType::Boolean, 0);
	LINA_FIELD(WidgetProps, borderThickness, "Border Thicknesses", FieldType::TBLR, 0);
	LINA_FIELD(WidgetProps, colorBorders, "Border Color", FieldType::Color, 0);
	LINA_FIELD(WidgetProps, childMargins, "Child Margins", FieldType::TBLR, 0);
	LINA_FIELD(WidgetProps, childPadding, "Child Padding", FieldType::Float, 0);
	LINA_FIELD(WidgetProps, customClip, "Custom Clip", FieldType::Boolean, 0);
	LINA_FIELD(WidgetProps, customClipRect, "Custom Clip Rect", FieldType::Rect, 0);
	LINA_FIELD_VEC(WidgetProps, onlyRound, "Only Round", FieldType::Int32, int32, 0);
	LINA_FIELD(WidgetProps, dropshadow, "Dropshadow Props", FieldType::UserClass, GetTypeID<DropshadowProps>());

	LINA_FIELD_DEPENDENCY_POS(WidgetProps, backgroundIsCentralGradient, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, interpolateColor, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, colorInterpolateSpeed, "interpolateColor", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, colorBackgroundDirection, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, colorBackground, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, colorOutline, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, colorOutlineControls, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, colorDisabled, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, hoveredIsDifferentColor, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, colorHovered, "hoveredIsDifferentColor", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, pressedIsDifferentColor, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, colorPressed, "pressedIsDifferentColor", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, outlineThickness, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, outlineIsInner, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, rounding, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, fitTexture, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, textureTiling, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, activeTextureTiling, "drawBackground", 1);
	LINA_FIELD_DEPENDENCY_POS(WidgetProps, customClipRect, "customClip", 1);

	LINA_CLASS_END(WidgetProps)

} // namespace Lina
