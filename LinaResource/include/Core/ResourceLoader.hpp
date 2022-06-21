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

/*
Class: ResourceLoader

Represents a bundle, containing various packages for resource categories. Raw packages contain RAW data loaded from .linapackage files.

Timestamp: 12/22/2020 12:26:55 AM
*/

#pragma once

#ifndef ResourceBundle_HPP
#define ResourceBundle_HPP

// Headers here.
#include "Core/ResourceCommon.hpp"
#include "Core/CommonUtility.hpp"
#include "Data/PriorityQueue.hpp"

namespace Lina
{
    namespace Event
    {
        class EventSystem;
    }
} // namespace Lina

namespace Lina::Resources
{
    class MemoryEntry
    {
    public:
        MemoryEntry() = default;
        ~MemoryEntry();

        MemoryEntry(int priority, const String& path, Vector<unsigned char>& data)
        {
            m_priority = priority;
            m_path     = path;
            m_data     = data;
        }

        int                   m_priority = 100;
        String                m_path     = "";
        Vector<unsigned char> m_data;
    };

    struct CompareMemEntry
    {
        bool operator()(MemoryEntry const& p1, MemoryEntry const& p2)
        {
            // return "true" if "p1" is ordered
            // before "p2", for example:
            return p1.m_priority > p2.m_priority;
        }
    };

    class FileEntry
    {
    public:
        FileEntry()  = default;
        ~FileEntry() = default;

        FileEntry(int priority, Utility::File* file)
        {
            m_priority = priority;
            m_file     = file;
        }

        Utility::File* m_file     = nullptr;
        int            m_priority = 100;
    };

    struct CompareFileEntry
    {
        bool operator()(FileEntry const& p1, FileEntry const& p2)
        {
            // return "true" if "p1" is ordered
            // before "p2", for example:
            return p1.m_priority > p2.m_priority;
        }
    };

    typedef PriorityQueue<MemoryEntry, Vector<MemoryEntry>, CompareMemEntry> MemoryQueue;
    typedef PriorityQueue<FileEntry, Vector<FileEntry>, CompareFileEntry>    FileQueue;

    class ResourceLoader
    {

    public:
        /// <summary>
        /// Loads a single file resource.
        /// </summary>
        bool LoadSingleResourceFromFile(TypeID tid, const String& fullpath);

    private:

        friend class ResourcePackager;

        /// <summary>
        /// Stores the given memory resource into a priority queue.
        /// </summary>
        void PushResourceFromMemory(const String& path, Vector<unsigned char>& data);

        /// <summary>
        /// Loads all memory buffers stored in memory resource queue. Use PushResourceFromMemory to push resources in.
        /// </summary>
        void LoadAllMemoryResources();

        /// <summary>
        /// Scans the given file for resources and saves the contents into m_fileResources priority queue.
        /// </summary>
        void ScanResourcesInFolder(Utility::Folder* folder);

        /// <summary>
        /// Loads all the resources in m_fileResources queue. Use ScanResourcesInFolder to push resources in.
        /// </summary>
        void LoadAllScannedResources();

        MemoryQueue m_memoryResources;
        FileQueue   m_fileResources;
        TypeID      m_lastResourceTypeID   = -1;
        int         m_lastResourcePriority = 0;
    };
} // namespace Lina::Resources

#endif
