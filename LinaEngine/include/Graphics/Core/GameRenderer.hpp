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

#ifndef GameRenderer_HPP
#define GameRenderer_HPP

#include "Renderer.hpp"
#include "RenderData.hpp"
#include "Graphics/PipelineObjects/CommandPool.hpp"
#include "Graphics/PipelineObjects/CommandBuffer.hpp"
#include "Graphics/PipelineObjects/DescriptorPool.hpp"
#include "View.hpp"
#include "DrawPass.hpp"
#include "Data/IDList.hpp"

namespace Lina
{
    namespace Editor
    {
        class Editor;
    }
    namespace Event
    {
        struct ELevelUninstalled;
        struct ELevelInstalled;
        struct EComponentCreated;
        struct EComponentDestroyed;
        struct EPreMainLoop;
        struct EWindowResized;
        struct EWindowPositioned;
        struct EResourceLoaded;
    } // namespace Event

} // namespace Lina

namespace Lina::Graphics
{
    class GameRenderer : public Renderer
    {
    public:
        GameRenderer()          = default;
        virtual ~GameRenderer() = default;

        inline uint32 GetFrameIndex()
        {
            return m_frameNumber % FRAMES_IN_FLIGHT;
        }

        struct RenderWorldData
        {
            Texture*                     finalTexture = nullptr;
            IDList<RenderableComponent*> allRenderables;
            Vector<RenderableData>       extractedRenderables;
            DrawPass                     opaquePass;
            View                         playerView;
            CameraComponent*             cameraComponent = nullptr;
            GPUSceneData                 sceneData;
            GPULightData                 lightData;
        };

    protected:
        virtual void Initialize(Swapchain* swp, GUIBackend* guiBackend) override;
        virtual void Shutdown() override;
        virtual void Tick() override;
        virtual void Render() override;
        virtual void SyncData() override;
        virtual void Stop() override;
        virtual void Join() override;
        virtual void OnLevelUninstalled(const Event::ELevelUninstalled& ev);
        virtual void OnLevelInstalled(const Event::ELevelInstalled& ev);
        virtual void OnComponentCreated(const Event::EComponentCreated& ev);
        virtual void OnComponentDestroyed(const Event::EComponentDestroyed& ev);
        virtual void OnPreMainLoop(const Event::EPreMainLoop& ev);
        virtual void OnWindowResized(const Event::EWindowResized& ev);
        virtual void OnWindowPositioned(const Event::EWindowPositioned& newPos);
        virtual void OnResourceLoaded(const Event::EResourceLoaded& res);
        virtual void HandleOutOfDateImage();
        virtual void MergeMeshes();
        virtual void CreateRenderPasses();

    protected:
        friend class Editor::Editor;

        CommandPool                           m_cmdPool;
        uint32                                m_frameNumber = 0;
        Frame                                 m_frames[FRAMES_IN_FLIGHT];
        DescriptorPool                        m_descriptorPool;
        Vector<CommandBuffer>                 m_cmds;
        Vector<GPUObjectData>                 m_gpuObjectData;
        RenderWorldData                       m_renderWorldData;
        HashMap<Mesh*, MergedBufferMeshEntry> m_meshEntries;
        Vector<StringID>                      m_mergedModelIDs;
        Buffer                                m_cpuVtxBuffer;
        Buffer                                m_cpuIndexBuffer;
        Buffer                                m_gpuVtxBuffer;
        Buffer                                m_gpuIndexBuffer;
        bool                                  m_recreateSwapchain = false;
        bool                                  m_hasLevelLoaded    = false;
        Atomic<bool>                          m_stopped           = false;
    };
} // namespace Lina::Graphics

#endif
