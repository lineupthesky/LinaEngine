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

#ifndef Resource_HPP
#define Resource_HPP

// Headers here.
#include "Data/String.hpp"
#include "Data/Streams.hpp"
#include "Utility/StringId.hpp"
#include "Serialization/Archive.hpp"

namespace Lina
{

    namespace Memory
    {
        class MemoryManager;
    }
} // namespace Lina
namespace Lina::Resources
{
    class EditorResourceLoader;

    class Resource
    {
    public:
        Resource()          = default;
        virtual ~Resource() = default;

        virtual Resource* LoadFromFile(const String& path)                         = 0;
        virtual Resource* LoadFromMemory(Serialization::Archive<IStream>& archive) = 0;
        virtual void      WriteToPackage(Serialization::Archive<OStream>& archive) = 0;
        virtual void      LoadReferences(){};
        virtual void      SaveToFile(){};

        inline TypeID GetTID()
        {
            return m_tid;
        }

        inline StringID GetSID()
        {
            return m_sid;
        }

        inline String& GetPath()
        {
            return m_path;
        }

    protected:
        virtual void SaveAssetData();
        virtual void LoadAssetData();
        virtual void SaveToArchive(Serialization::Archive<OStream>& archive){};
        virtual void LoadFromArchive(Serialization::Archive<IStream>& archive){};

        Serialization::Archive<IStream> GetMetaArchive();
        void                            SaveMetaArchive(Serialization::Archive<OStream>& arch);
        bool                            MetaArchiveExists();

    protected:
        template <typename U>
        friend class ResourceCache;
        friend class Memory::MemoryManager;
        friend class EditorResourceLoader;
        friend class ResourceLoader;
        friend class ResourcePackager;

        uint32   m_allocPoolIndex = 0;
        TypeID   m_tid            = 0;
        StringID m_sid            = 0;
        String   m_path           = "";
    };
} // namespace Lina::Resources

#endif
