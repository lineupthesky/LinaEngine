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

#include "Core/Resources/ResourceManager.hpp"
#include "Core/Application.hpp"
#include "Core/ApplicationDelegate.hpp"
#include "Core/Resources/Resource.hpp"
#include "Core/Meta/ProjectData.hpp"
#include "Core/Application.hpp"
#include "Core/Resources/ResourceManagerListener.hpp"
#include "Common/Serialization/Serialization.hpp"
#include "Common/Data/CommonData.hpp"
#include "Common/Data/HashSet.hpp"
#include "Common/FileSystem/FileSystem.hpp"

#include "Common/Platform/PlatformTime.hpp"
#include "Common/Math/Math.hpp"
#include "Common/System/SystemInfo.hpp"

namespace Lina
{
	void ResourceManagerV2::Shutdown()
	{

		for (auto [tid, cache] : m_caches)
		{
			Vector<Resource*> resources = cache->GetAllResources();

			for (Resource* res : resources)
			{
				if (res->IsHWValid())
					res->DestroyHW();
			}

			delete cache;
		}
	}

	void ResourceManagerV2::ReloadResourceHW(const HashSet<Resource*>& resources)
	{
		CheckLock();

		Application::GetLGX()->Join();

		for (ResourceManagerListener* l : m_listeners)
			l->OnResourceManagerPreDestroyHW(resources);

		for (Resource* res : resources)
		{
			res->DestroyHW();
			res->GenerateHW();
		}

		for (ResourceManagerListener* l : m_listeners)
			l->OnResourceManagerGeneratedHW(resources);
	}

	void ResourceManagerV2::UnloadResourceSpace(StringID id)
	{
		HashSet<Resource*>& resources = m_resourceSpaces.at(id);

		ResourceDefinitionList unloadList;

		for (Resource* res : resources)
		{
			bool foundInAnotherSpace = false;

			for (const auto& space : m_resourceSpaces)
			{
				if (space.first == id)
					continue;

				auto it = linatl::find_if(space.second.begin(), space.second.end(), [res](Resource* r) -> bool { return r == res; });

				if (it != space.second.end())
				{
					foundInAnotherSpace = true;
					break;
				}
			}

			if (!foundInAnotherSpace)
			{
				unloadList.insert({
					.id	 = res->GetID(),
					.tid = res->GetTID(),
				});
			}
		}

		resources.clear();
		UnloadResources(unloadList);
	}

	void ResourceManagerV2::AddListener(ResourceManagerListener* listener)
	{
		m_listeners.push_back(listener);
	}

	void ResourceManagerV2::RemoveListener(ResourceManagerListener* listener)
	{
		auto it = linatl::find_if(m_listeners.begin(), m_listeners.end(), [listener](ResourceManagerListener* l) -> bool { return l == listener; });
		m_listeners.erase(it);
	}

	HashSet<Resource*> ResourceManagerV2::LoadResourcesFromFile(const ResourceDefinitionList& resourceDefs, Delegate<void(uint32 loaded, const ResourceDef& currentItem)> onProgress, uint64 resourceSpace)
	{
		CheckLock();

		HashSet<Resource*> resources;

		uint32 idx = 0;
		for (const ResourceDef& def : resourceDefs)
		{
			auto cache = GetCache(def.tid);

			Resource* res = cache->GetIfExists(def.id);

			if (res)
			{
				if (onProgress)
					onProgress(++idx, def);

				m_resourceSpaces[resourceSpace].insert(res);

				resources.insert(res);
				continue;
			}

			res = cache->Create(def.id, def.name);

			m_resourceSpaces[resourceSpace].insert(res);

			if (def.customMeta.GetCurrentSize() != 0)
			{
				IStream stream;
				stream.Create(def.customMeta.GetDataRaw(), def.customMeta.GetCurrentSize());
				res->SetCustomMeta(stream);
				stream.Destroy();
			}

			if (!res->LoadFromFile(def.name))
			{
				cache->Destroy(def.id);
				LINA_ERR("[Resource] -> Failed loading resource: {0}", def.name);

				if (onProgress)
					onProgress(++idx, def);
				continue;
			}

			res->SetID(def.id);
			res->SetName(def.name);
			res->GenerateHW();
			resources.insert(res);
			LINA_TRACE("[Resource] -> Loaded resource: {0}", def.name);

			if (onProgress)
				onProgress(++idx, def);
		}

		for (ResourceManagerListener* l : m_listeners)
			l->OnResourceManagerGeneratedHW(resources);

		return resources;
	}

	HashSet<Resource*> ResourceManagerV2::LoadResourcesFromProject(ProjectData* project, const HashSet<ResourceID>& resources, Delegate<void(uint32 loaded, Resource* currentItem)> onProgress, uint64 resourceSpace)
	{
		CheckLock();

		HashSet<Resource*> loaded;

		uint32 idx = 0;
		for (ResourceID id : resources)
		{
			ResourceDirectory* dir = project->GetResourceRoot().FindResourceDirectory(id);

			if (dir == nullptr)
			{
				LINA_ERR("Can't find resource to load from project! {0}", id);

				if (onProgress)
					onProgress(++idx, nullptr);

				continue;
			}

			auto cache = GetCache(dir->resourceTID);

			Resource* res = cache->GetIfExists(id);

			if (res != nullptr)
			{
				if (onProgress)
					onProgress(++idx, res);

				m_resourceSpaces[resourceSpace].insert(res);

				loaded.insert(res);
				continue;
			}

			res = cache->Create(id, dir->name);

			m_resourceSpaces[resourceSpace].insert(res);

			IStream stream = Serialization::LoadFromFile(project->GetResourcePath(id).c_str());

			if (stream.Empty())
			{
				cache->Destroy(id);
				LINA_ERR("[Resource] -> Failed loading resource: {0}", id);

				if (onProgress)
					onProgress(++idx, res);

				continue;
			}

			res->LoadFromStream(stream);
			stream.Destroy();

			res->GenerateHW();
			loaded.insert(res);

			if (onProgress)
				onProgress(++idx, res);

			LINA_TRACE("[Resource] -> Loaded resource: {0}", res->GetPath());
		}

		for (ResourceManagerListener* l : m_listeners)
			l->OnResourceManagerGeneratedHW(loaded);

		return loaded;
	}

	void ResourceManagerV2::UnloadResources(const ResourceDefinitionList& resources)
	{
		CheckLock();

		HashSet<Resource*> toDestroy;

		for (const ResourceDef& def : resources)
		{
			ResourceCacheBase* cache = GetCache(def.tid);
			Resource*		   r	 = cache->GetIfExists(def.id);
			if (r == nullptr)
				continue;

			toDestroy.insert(r);
		}

		for (ResourceManagerListener* l : m_listeners)
			l->OnResourceManagerPreDestroyHW(toDestroy);

		for (Resource* res : toDestroy)
		{
			res->DestroyHW();
			ResourceCacheBase* cache = GetCache(res->GetTID());
			cache->Destroy(res->GetID());
		}
	}

	ResourceCacheBase* ResourceManagerV2::GetCache(TypeID tid)
	{
		auto			   it	 = m_caches.find(tid);
		ResourceCacheBase* cache = nullptr;
		if (it == m_caches.end())
		{
			MetaType& type = ReflectionSystem::Get().Resolve(tid);
			void*	  ptr  = type.GetFunction<void*()>("CreateResourceCache"_hs)();
			cache		   = static_cast<ResourceCacheBase*>(ptr);
			m_caches[tid]  = cache;
		}
		else
			cache = it->second;

		return cache;
	}

	void ResourceManagerV2::CheckLock()
	{
		LINA_ASSERT(!m_locked && SystemInfo::IsMainThread(), "Resources can only be modified inside main thread and outside of resource lock!");
	}
} // namespace Lina
