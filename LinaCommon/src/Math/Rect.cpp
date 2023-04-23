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

#include "Math/Rect.hpp"

namespace Lina
{
	Rect::Rect(const Recti& r)
	{
		pos	 = r.pos;
		size = r.size;
	}

	Rect Rect::Shrink(float percentage) const
	{
		Rect		r	 = *this;
		const float amtX = r.size.x * percentage / 100.0f;
		const float amtY = r.size.y * percentage / 100.0f;
		r.size -= Vector2(amtX, amtY) * 2.0f;
		r.pos += Vector2(amtX, amtY);
		return r;
	}

	bool Rect::IsPointInside(const Vector2& p) const
	{
		return p.x > pos.x && p.x < pos.x + size.x && p.y > pos.y && p.y < pos.y + size.y;
	}

	bool Rect::IsInBorder(const Vector2& p, float borderThickness, int& borderPosition) const
	{
		if (p.x >= pos.x - borderThickness && p.x <= pos.x + borderThickness && p.y >= pos.y - borderThickness && p.y <= pos.y + borderThickness)
		{
			borderPosition = 0; // TopLeft
			return true;
		}
		else if (p.x >= pos.x + size.x - borderThickness && p.x <= pos.x + size.x + borderThickness && p.y >= pos.y - borderThickness && p.y <= pos.y + borderThickness)
		{
			borderPosition = 1; // TopRight
			return true;
		}
		else if (p.y >= pos.y - borderThickness && p.y <= pos.y + borderThickness && p.x > pos.x + borderThickness && p.x < pos.x + size.x - borderThickness)
		{
			borderPosition = 2; // Top
			return true;
		}
		else if (p.x >= pos.x + size.x - borderThickness && p.x <= pos.x + size.x + borderThickness && p.y > pos.y + borderThickness && p.y < pos.y + size.y - borderThickness)
		{
			borderPosition = 3; // Right
			return true;
		}
		else if (p.x >= pos.x + size.x - borderThickness && p.x <= pos.x + size.x + borderThickness && p.y >= pos.y + size.y - borderThickness && p.y <= pos.y + size.y + borderThickness)
		{
			borderPosition = 4; // BottomRight
			return true;
		}
		else if (p.y >= pos.y + size.y - borderThickness && p.y <= pos.y + size.y + borderThickness && p.x > pos.x + borderThickness && p.x < pos.x + size.x - borderThickness)
		{
			borderPosition = 5; // Bottom
			return true;
		}
		else if (p.x >= pos.x - borderThickness && p.x <= pos.x + borderThickness && p.y > pos.y + borderThickness && p.y < pos.y + size.y - borderThickness)
		{
			borderPosition = 6; // Left
			return true;
		}
		else if (p.x >= pos.x - borderThickness && p.x <= pos.x + borderThickness && p.y >= pos.y + size.y - borderThickness && p.y <= pos.y + size.y + borderThickness)
		{
			borderPosition = 7; // BottomLeft
			return true;
		}

		borderPosition = -1;
		return false;
	}

	Vector2 Rect::GetCenter()
	{
		return Vector2(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f);
	}

	bool Recti::IsPointInside(const Vector2i& p) const
	{
		return p.x > pos.x && p.x < pos.x + size.x && p.y > pos.y && p.y < pos.y + size.y;
	}

	bool Recti::IsInBorder(const Vector2& p, int borderThickness, int& borderPosition) const
	{
		if (p.x >= pos.x - borderThickness && p.x <= pos.x + borderThickness && p.y >= pos.y - borderThickness && p.y <= pos.y + borderThickness)
		{
			borderPosition = 0; // TopLeft
			return true;
		}
		else if (p.x >= pos.x + size.x - borderThickness && p.x <= pos.x + size.x + borderThickness && p.y >= pos.y - borderThickness && p.y <= pos.y + borderThickness)
		{
			borderPosition = 1; // TopRight
			return true;
		}
		else if (p.y >= pos.y - borderThickness && p.y <= pos.y + borderThickness && p.x > pos.x + borderThickness && p.x < pos.x + size.x - borderThickness)
		{
			borderPosition = 2; // Top
			return true;
		}
		else if (p.x >= pos.x + size.x - borderThickness && p.x <= pos.x + size.x + borderThickness && p.y > pos.y + borderThickness && p.y < pos.y + size.y - borderThickness)
		{
			borderPosition = 3; // Right
			return true;
		}
		else if (p.x >= pos.x + size.x - borderThickness && p.x <= pos.x + size.x + borderThickness && p.y >= pos.y + size.y - borderThickness && p.y <= pos.y + size.y + borderThickness)
		{
			borderPosition = 4; // BottomRight
			return true;
		}
		else if (p.y >= pos.y + size.y - borderThickness && p.y <= pos.y + size.y + borderThickness && p.x > pos.x + borderThickness && p.x < pos.x + size.x - borderThickness)
		{
			borderPosition = 5; // Bottom
			return true;
		}
		else if (p.x >= pos.x - borderThickness && p.x <= pos.x + borderThickness && p.y > pos.y + borderThickness && p.y < pos.y + size.y - borderThickness)
		{
			borderPosition = 6; // Left
			return true;
		}
		else if (p.x >= pos.x - borderThickness && p.x <= pos.x + borderThickness && p.y >= pos.y + size.y - borderThickness && p.y <= pos.y + size.y + borderThickness)
		{
			borderPosition = 7; // BottomLeft
			return true;
		}

		borderPosition = -1;
		return false;
	}

	Vector2i Recti::GetCenter()
	{
		return Vector2i(pos.x + size.x / 2, pos.y + size.x / 2);
	}

} // namespace Lina