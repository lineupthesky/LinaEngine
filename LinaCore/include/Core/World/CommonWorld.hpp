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

#ifndef WorldCommon_HPP
#define WorldCommon_HPP

#include "Common/Data/Bitmask.hpp"

namespace Lina
{

	typedef uint64 EntityID;

	enum EntityFlags
	{
		EF_NONE = 0,
	};

	enum ComponentFlags
	{
		CF_NONE				   = 1 << 0,
		CF_RENDERABLE		   = 1 << 1,
		CF_RECEIVE_TICK		   = 1 << 2,
		CF_RECEIVE_EDITOR_TICK = 1 << 3,
	};

	enum WorldFlags
	{
		WORLD_FLAGS_NONE = 1 << 0,
	};

	enum class ComponentEventType
	{
		Create,
		Destroy,
		PlayBegin,
		PlayEnd,
		PreTick,
		Tick,
		PostTick,
	};

	struct ComponentEvent
	{
		ComponentEventType type = ComponentEventType::Create;
		float			   data = 0.0f;
	};

} // namespace Lina

#endif
