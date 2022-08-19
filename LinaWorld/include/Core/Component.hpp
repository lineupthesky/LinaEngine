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

#ifndef Component_HPP
#define Component_HPP

#include "Core/SizeDefinitions.hpp"
#include "ComponentCache.hpp"
#include <cereal/archives/portable_binary.hpp>

namespace Lina::World
{
    class Entity;
    // Actual game state
    class Component
    {
    public:
        Component()  = default;
        ~Component() = default;

        virtual TypeID GetTID() = 0;
        virtual void   SaveToArchive(cereal::PortableBinaryOutputArchive& oarchive);
        virtual void   LoadFromArchive(cereal::PortableBinaryInputArchive& iarchive);

    private:

        friend class World;
        friend class Entity;

        uint32  m_entityID = 0;
        Entity* m_entity   = nullptr;
    };

} // namespace Lina::World

#endif
