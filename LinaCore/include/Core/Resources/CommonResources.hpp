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

#ifndef CommonResources_HPP
#define CommonResources_HPP

#include "Common/StringID.hpp"
#include "Common/Data/String.hpp"
#include "Common/Common.hpp"

namespace Lina
{
	enum class PackageType
	{
		Default,
		Package1,
		Package2,
		PackageLevels,
	};

	extern String GGetPackagePath(PackageType pt);

	struct ResourceIdentifier
	{
		ResourceIdentifier() = default;
		ResourceIdentifier(const String& path, TypeID tid, StringID sid, bool useCustomMeta = false)
		{
			this->path			= path;
			this->sid			= sid;
			this->tid			= tid;
			this->useCustomMeta = useCustomMeta;
		}

		TypeID	 tid		   = 0;
		StringID sid		   = 0;
		String	 path		   = "";
		bool	 useCustomMeta = false;

		void SaveToStream(OStream& stream) const;
		void LoadFromStream(IStream& stream);
	};

	struct ResourceLoadTask
	{
		Taskflow				   tf;
		Vector<ResourceIdentifier> identifiers;
		Atomic<bool>			   isCompleted = false;
		int32					   id		   = 0;
		uint64					   startTime   = 0;
		uint64					   endTime	   = 0;
	};

} // namespace Lina

#endif