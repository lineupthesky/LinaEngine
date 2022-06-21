#include "ECS/Registry.hpp"
#include "ECS/Components/EntityDataComponent.hpp"
#include "EventSystem/EventSystem.hpp"
#include "EventSystem/ECSEvents.hpp"
#include "Log/Log.hpp"
#include <entt/meta/resolve.hpp>
#include <entt/meta/meta.hpp>
#include <entt/entity/snapshot.hpp>

namespace Lina::ECS
{
    template <typename Type>
    void SerComponent(entt::snapshot& snapshot, cereal::PortableBinaryOutputArchive& archive)
    {
        snapshot.component<Type>(archive);
    }

    void Sa(entt::snapshot& snapshot)
    {
    }
    using namespace entt::literals;

    Registry* Registry::s_ecs = nullptr;

    void Registry::SerializeComponentsInRegistry(cereal::PortableBinaryOutputArchive& archive)
    {
        auto& snapshot = entt::snapshot{*this};
        snapshot.entities(archive);

        auto& it = storage();
        for (auto& store : it)
        {
            auto resolved = entt::resolve(store.first);
            if (resolved)
            {
                auto& serializeFunc = resolved.func("serialize"_hs);
                if (serializeFunc)
                    serializeFunc.invoke({}, entt::forward_as_meta(snapshot), entt::forward_as_meta(archive));
            }
        }
    }

    void Registry::DeserializeComponentsInRegistry(cereal::PortableBinaryInputArchive& archive)
    {
        auto& loader = entt::snapshot_loader{*this};
        loader.entities(archive);

        auto& it = storage();
        for (auto& store : it)
        {
            auto resolved = entt::resolve(store.first);

            if (resolved)
            {
                auto& deserializeFunc = resolved.func("deserialize"_hs);

                if (deserializeFunc)
                    deserializeFunc.invoke({}, entt::forward_as_meta(loader), entt::forward_as_meta(archive));
            }
        }
    }

    void Registry::AddChildToEntity(Entity parent, Entity child)
    {

        if (parent == child)
            return;

        EntityDataComponent& childData  = get<EntityDataComponent>(child);
        EntityDataComponent& parentData = get<EntityDataComponent>(parent);

        const Vector3    childGlobalPos   = childData.GetLocation();
        const Quaternion childGlobalRot   = childData.GetRotation();
        const Vector3    childGlobalScale = childData.GetScale();
        if (parentData.m_parent == child || childData.m_parent == parent)
            return;

        if (childData.m_parent != entt::null)
        {
            RemoveChildFromEntity(childData.m_parent, child);
        }

        parentData.m_children.emplace(child);
        childData.m_parent = parent;

        // Adding a child to an entity does not change any transformation
        // Due to this, child's local values will be according to the previous parent if exists.
        // So we update the global transformation, which makes sure local transformations are set according to
        // the current parent.
        childData.SetLocation(childGlobalPos);
        childData.SetRotation(childGlobalRot);
        childData.SetScale(childGlobalScale);
    }

    void Registry::DestroyAllChildren(Entity parent)
    {
        EntityDataComponent* data = try_get<EntityDataComponent>(parent);

        if (data == nullptr)
            return;

        int                        counter  = 0;
        Set<Entity>           children = data->m_children;
        Set<Entity>::iterator it;
        for (it = children.begin(); it != children.end(); ++it)
        {
            DestroyEntity(*it);
            counter++;
        }
        data->m_children.clear();
    }

    void Registry::RemoveChildFromEntity(Entity parent, Entity child)
    {
        Set<Entity>& children = get<EntityDataComponent>(parent).m_children;
        if (children.find(child) != children.end())
        {
            children.erase(child);
        }

        get<EntityDataComponent>(child).m_parent = entt::null;
    }

    void Registry::RemoveFromParent(Entity child)
    {
        Entity parent = get<EntityDataComponent>(child).m_parent;

        if (parent != entt::null)
            RemoveChildFromEntity(parent, child);
    }

    void Registry::CloneEntity(Entity from, Entity to)
    {
        auto& it = storage();
        for (auto& store : it)
        {
            for (auto ent : store.second)
            {
                if (from == ent)
                {
                    auto resolved = entt::resolve(store.first);

                    if (resolved)
                    {
                        auto& cloneFunc = resolved.func("clone"_hs);

                        if (cloneFunc)
                            cloneFunc.invoke({}, from, to);
                    }
                    break;
                }
            }
        }
    }

    const Set<Entity>& Registry::GetChildren(Entity parent)
    {
        return get<EntityDataComponent>(parent).m_children;
    }

    Entity Registry::CreateEntity(const String& name)
    {
        entt::entity ent = create();
        emplace<EntityDataComponent>(ent, EntityDataComponent(true, true, name));
        return ent;
    }

    Entity Registry::CreateEntity(Entity source, bool attachParent)
    {
        EntityDataComponent sourceData = get<EntityDataComponent>(source);

        // Build the entity.
        Entity copy = create();

        // Copy entity components to newly created one
        CloneEntity(source, copy);

        EntityDataComponent& copyData = get<EntityDataComponent>(copy);
        copyData.m_parent             = entt::null;
        copyData.m_children.clear();

        for (Entity child : sourceData.m_children)
        {
            Entity               copyChild     = CreateEntity(child, false);
            EntityDataComponent& copyChildData = get<EntityDataComponent>(copyChild);
            copyChildData.m_parent             = copy;
            get<EntityDataComponent>(copy).m_children.emplace(copyChild);
        }

        if (attachParent && sourceData.m_parent != entt::null)
            AddChildToEntity(sourceData.m_parent, copy);

        return copy;
    }

    Entity Registry::GetEntity(const String& name)
    {
        auto singleView = view<EntityDataComponent>();

        for (auto entity : singleView)
        {
            if (singleView.get<EntityDataComponent>(entity).m_name.compare(name) == 0)
                return entity;
        }

        LINA_WARN("Entity with the name {0} could not be found, returning null entity.", name);
        return entt::null;
    }

    void Registry::DestroyEntity(Entity entity, bool isRoot)
    {
        Set<Entity> toErase;
        for (Entity child : get<EntityDataComponent>(entity).m_children)
        {
            toErase.emplace(child);
            DestroyEntity(child, false);
        }

        for (Entity child : toErase)
            get<EntityDataComponent>(entity).m_children.erase(child);

        if (isRoot)
            RemoveFromParent(entity);

        destroy(entity);
    }

    void Registry::SetEntityEnabled(Entity entity, bool isEnabled)
    {
        auto& data = get<EntityDataComponent>(entity);
        if (data.m_isEnabled == isEnabled)
            return;

        data.m_isEnabled = isEnabled;
        Event::EventSystem::Get()->Trigger<Event::EEntityEnabledChanged>(Event::EEntityEnabledChanged{entity, isEnabled});

        for (auto child : data.m_children)
        {
            auto& childData = get<EntityDataComponent>(child);

            if (!isEnabled)
            {
                childData.m_wasPreviouslyEnabled = childData.m_isEnabled;
                SetEntityEnabled(child, false);
            }
            else
            {
                if (childData.m_wasPreviouslyEnabled)
                    SetEntityEnabled(child, true);
            }
        }
    }

} // namespace Lina::ECS
