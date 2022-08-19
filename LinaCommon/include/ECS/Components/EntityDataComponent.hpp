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

#ifndef EntityDataComponent_HPP
#define EntityDataComponent_HPP

// Headers here.
#include "Core/CommonECS.hpp"
#include "ECS/Component.hpp"
#include "Math/Transformation.hpp"

#include "Data/Serialization/SetSerialization.hpp"
#include "Data/Serialization/StringSerialization.hpp"

namespace Lina
{
    namespace Physics
    {
        class PhysicsEngine;
    } // namespace Physics
};    // namespace Lina

namespace Lina::ECS
{
    //LINA_COMPONENT("Entity Data Component", "ICON_FA_DATABASE", "", "false", "false")
    struct EntityDataComponent : public Component
    {
        EntityDataComponent(bool enabled, bool serialized, String name)
        {
            m_isEnabled            = enabled;
            m_serialized           = serialized;
            m_name                 = name;
            m_wasPreviouslyEnabled = m_isEnabled;
        }

        EntityDataComponent() = default;

        virtual void SetIsEnabled(bool isEnabled) override;
        /* TRANSFORM OPERATIONS */

        Matrix ToMatrix() const
        {
            return m_transform.ToMatrix();
        }
        Matrix ToLocalMatrix() const
        {
            return m_transform.ToLocalMatrix();
        }
        void SetTransformation(Matrix& mat, bool omitScale = false);
        void SetLocalTransformation(Matrix& mat, bool omitScale = false);

        void AddRotation(const Vector3& angles);
        void AddLocalRotation(const Vector3& angles);
        void AddPosition(const Vector3& loc);
        void AddLocalPosition(const Vector3& loc);

        Transformation GetInterpolated(float interpolation);
        void           SetLocalPosition(const Vector3& loc);
        void           SetPosition(const Vector3& loc);
        void           SetLocalRotation(const Quaternion& rot, bool isThisPivot = true);
        void           SetLocalRotationAngles(const Vector3& angles, bool isThisPivot = true);
        void           SetRotation(const Quaternion& rot, bool isThisPivot = true);
        void           SetRotationAngles(const Vector3& angles, bool isThisPivot = true);
        void           SetLocalScale(const Vector3& scale, bool isThisPivot = true);
        void           SetScale(const Vector3& scale, bool isThisPivot = true);

        const Vector3& GetLocalRotationAngles()
        {
            return m_transform.m_localRotationAngles;
        }
        const Vector3& GetLocalPosition()
        {
            return m_transform.m_localPosition;
        }
        const Quaternion& GetLocalRotation()
        {
            return m_transform.m_localRotation;
        }
        const Vector3& GetLocalScale()
        {
            return m_transform.m_localScale;
        }
        const Vector3& GetPosition()
        {
            return m_transform.m_position;
        }
        const Quaternion& GetRotation()
        {
            return m_transform.m_rotation;
        }
        const Vector3& GetRotationAngles()
        {
            return m_transform.m_rotationAngles;
        }
        const Vector3& GetScale()
        {
            return m_transform.m_scale;
        }

    private:
        void UpdateGlobalLocation();
        void UpdateLocalLocation();
        void UpdateGlobalRotation();
        void UpdateLocalRotation();
        void UpdateGlobalScale();
        void UpdateLocalScale();

    private:
        friend class cereal::access;
        friend class Registry;
        friend class Physics::PhysicsEngine;

        bool           m_serialized = true;
        String         m_name       = "";
        Set<Entity>    m_children;
        Entity         m_parent               = entt::null;
        bool           m_isTransformLocked    = false;
        bool           m_wasPreviouslyEnabled = false;
        Transformation m_transform;

        template <class Archive>
        void serialize(Archive& archive)
        {
            archive(m_transform, m_isTransformLocked, m_isEnabled, m_wasPreviouslyEnabled, m_name, m_parent, m_children);
        }
    };
} // namespace Lina::ECS

#endif
