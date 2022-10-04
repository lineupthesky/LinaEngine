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

#ifndef CommonTypesSerialization_HPP
#define CommonTypesSerialization_HPP

#include "Archive.hpp"
#include "Data/DataCommon.hpp"
#include <variant>

#include <cereal/types/variant.hpp>

namespace Lina::Serialization
{
    template<typename A, typename B>
    struct Serialize_NonTrivial<Archive<OStream>, Pair<A, B>>
    {
        template<typename Ar>
        void Serialize(Ar& ar, Pair<A, B>& p)
        {
            ar(p.first);
            ar(p.second);
        }
    };

    template<typename A, typename B>
    struct Serialize_NonTrivial<Archive<IStream>, Pair<A, B>>
    {
        template<typename Ar>
        void Serialize(Ar& ar, Pair<A, B>& p)
        {
           A first = A();
           B second = B();
           ar(first);
           ar(second);
           p.first = first;
           p.second = second;
        }
    };

}


#endif