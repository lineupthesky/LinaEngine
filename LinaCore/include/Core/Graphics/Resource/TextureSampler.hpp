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

#ifndef TextureSampler_HPP
#define TextureSampler_HPP

#include "Core/Resources/Resource.hpp"
#include "Common/Platform/LinaGXIncl.hpp"

namespace Lina
{
	class Renderer;

	class TextureSampler : public Resource
	{
	public:
		TextureSampler(ResourceManager* rm, bool isUserManaged, const String& path, StringID sid) : Resource(rm, isUserManaged, path, sid, GetTypeID<TextureSampler>()){};
		virtual ~TextureSampler();

		inline uint32 GetGPUHandle() const
		{
			return m_gpuHandle;
		}

		inline uint32 GetBindlessIndex() const
		{
			return m_bindlessIndex;
		}

	private:
		friend class GfxManager;

	protected:
		virtual void BatchLoaded() override;
		virtual void SaveToStream(OStream& stream) const override;
		virtual void LoadFromStream(IStream& stream) override;

	private:
		uint32				m_bindlessIndex = 0;
		uint32				m_gpuHandle		= 0;
		LinaGX::SamplerDesc m_samplerDesc	= {};
	};
} // namespace Lina

#endif