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

#include "Common/Data/String.hpp"
#include "Common/Data/ParameterCollection.hpp"
#include "Common/Serialization/Serializable.hpp"
#include "EditorLayout.hpp"
#include "Editor/CommonSettings.hpp"

namespace Lina::Editor
{
	// Version bumps
	// 0: initial

	class EditorSettings : public Serializable
	{
	public:
		static constexpr uint32 VERSION = 0;
		virtual void			SaveToStream(OStream& out) override;
		virtual void			LoadFromStream(IStream& in) override;

		inline const String& GetLastProjectPath() const
		{
			return m_lastProjectPath;
		}

		inline void SetLastProjectPath(const String& path)
		{
			m_lastProjectPath = path;
		}

		inline ResourceID GetLastWorldID() const
		{
			return m_lastWorldID;
		}

		inline void SetLastWorldID(ResourceID id)
		{
			m_lastWorldID = id;
		}

		inline EditorLayout& GetLayout()
		{
			return m_layout;
		}

		inline ParameterCollection& GetParams()
		{
			return m_paramCollection;
		}

		inline EditorInputSettings& GetInputSettings()
		{
			return m_inputSettings;
		}

	private:
		String		 m_lastProjectPath = "";
		ResourceID	 m_lastWorldID	   = 0;
		EditorLayout m_layout		   = {};

		ParameterCollection m_paramCollection;
		EditorInputSettings m_inputSettings;
	};

} // namespace Lina::Editor
