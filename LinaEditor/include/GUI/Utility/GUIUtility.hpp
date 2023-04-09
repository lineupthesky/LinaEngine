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

#ifndef GUIUtility_HPP
#define GUIUtility_HPP

#include "Core/SizeDefinitions.hpp"
#include "Core/Theme.hpp"

namespace Lina
{
	class Vector2;
	class Rect;
} // namespace Lina

namespace Lina::Editor
{
	class GUIUtility
	{
	public:
		static void DrawIcon(int threadID, float dpiScale, const char* icon, const Vector2& centerPos, Color tint = Color::White, int drawOrder = 0);
		static void DrawWindowBackground(int threadID, const Rect& rect, int drawOrder);
		static void DrawPopupBackground(int threadID, const Rect& rect, float borderThickness, int drawOrder);

		static bool IsInRect(const Vector2& pos, const Rect& rect);
	};
} // namespace Lina::Editor

#endif
