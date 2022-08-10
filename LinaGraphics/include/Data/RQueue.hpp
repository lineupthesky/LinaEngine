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

#ifndef GraphicsQueue_HPP
#define GraphicsQueue_HPP

#include "Core/GraphicsCommon.hpp"
#include "Data/HashMap.hpp"

struct VkQueue_T;

namespace Lina::Graphics
{

    class Fence;
    class Semaphore;
    class CommandBuffer;

    struct QueueFamilyIndices
    {
        uint32 graphicsFamily;
    };

    class RQueue
    {
    public:
        void Get(uint32 family);
        void Submit(const Semaphore& waitSemaphore, const Semaphore& signalSemaphore, const Fence& fence, const CommandBuffer& cmd, uint32 submitCount = 1);
        void Present(const Semaphore& waitSemaphore, uint32* swapchainImageIndex);

        // Runtime
        VkQueue_T* _ptr    = nullptr;
        uint32     _family = 0;

        static HashMap<uint32, uint32> s_queueCounterPerFamily;
    };
} // namespace Lina::Graphics

#endif