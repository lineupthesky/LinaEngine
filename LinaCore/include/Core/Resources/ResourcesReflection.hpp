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

#ifndef ResourcesReflection_HPP
#define ResourcesReflection_HPP

#include "ResourceManager.hpp"
#include "Common/Data/Vector.hpp"

namespace Lina
{
	class ResourceReflectionUtility
	{
	public:
		template <typename T> static inline void* CreateMock()
		{
			T* ptr = new T(nullptr, false, "", 0);
			return static_cast<void*>(ptr);
		}

		template <typename T> static inline void DestroyMock(void* ptr)
		{
			T* tptr = static_cast<T*>(ptr);
			delete tptr;
		}
	};
#define LINA_REGISTER_RESOURCE_TYPE(ClassName, ChunkCount, ExtensionVector, ResourcePackage)                                                                                                                                                                       \
	class ClassName;                                                                                                                                                                                                                                               \
	class ClassName##_LinaReflected                                                                                                                                                                                                                                \
	{                                                                                                                                                                                                                                                              \
	public:                                                                                                                                                                                                                                                        \
		ClassName##_LinaReflected()                                                                                                                                                                                                                                \
		{                                                                                                                                                                                                                                                          \
			Lina::ResourceManager::Get().RegisterResourceType<ClassName>(ChunkCount, ExtensionVector, ResourcePackage);                                                                                                                                            \
			Lina::ReflectionSystem::Get().Meta<ClassName>().AddFunction<void*()>(Lina::TO_SIDC("CreateMock"), std::bind(&Lina::ResourceReflectionUtility::CreateMock<ClassName>));                                                                                 \
			Lina::ReflectionSystem::Get().Meta<ClassName>().AddFunction<void(void*)>(Lina::TO_SIDC("DestroyMock"), std::bind(&Lina::ResourceReflectionUtility::DestroyMock<ClassName>, std::placeholders::_1));                                                    \
		}                                                                                                                                                                                                                                                          \
                                                                                                                                                                                                                                                                   \
	private:                                                                                                                                                                                                                                                       \
		static const ClassName##_LinaReflected ClassName##__;                                                                                                                                                                                                      \
	};                                                                                                                                                                                                                                                             \
	const ClassName##_LinaReflected ClassName##_LinaReflected::ClassName##__ = ClassName##_LinaReflected();

	//  REGISTER_RESOURCE_TYPE(test, 1, {"test"}, PackageType::Package1);
} // namespace Lina

#endif