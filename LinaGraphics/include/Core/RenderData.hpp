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

#ifndef RenderData_HPP
#define RenderData_HPP

#include "Math/AABB.hpp"
#include "Data/Vector.hpp"
#include "Data/Set.hpp"

namespace Lina::Graphics
{
    class RenderableComponent;
    class Mesh;
    class Material;

    struct VisibilityData
    {
        RenderableComponent* renderable = nullptr;
        Vector3              position   = Vector3::Zero;
        AABB                 aabb       = AABB();
        bool                 valid      = false;
    };

    struct GPUObjectData
    {
        Matrix modelMatrix = Matrix::Identity();
    };

    struct MeshMaterialPair
    {
        Mesh*     mesh     = nullptr;
        Material* material = nullptr;
    };

    struct RenderBatch
    {
        MeshMaterialPair pair;
        Set<uint32>      renderableIDs;
    };

} // namespace Lina::Graphics

#endif