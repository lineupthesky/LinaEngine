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

#include "Core/DrawPass.hpp"
#include "Core/View.hpp"
#include "Components/RenderableComponent.hpp"
#include "Core/RenderEngine.hpp"
#include "Core/Entity.hpp"
#include "Core/Renderer.hpp"
#include "Resource/Mesh.hpp"
#include "Resource/Material.hpp"
#include "Resource/ModelNode.hpp"
#include "Utility/Vulkan/VulkanUtility.hpp"

namespace Lina::Graphics
{

    void DrawPass::PrepareRenderData(Vector<RenderableData>& drawList)
    {
        ExtractVisibleRenderables(drawList);
        BatchVisibleRenderables();
    }

    void DrawPass::ExtractVisibleRenderables(Vector<RenderableData>& drawList)
    {
        PROFILER_FUNC(PROFILER_THREAD_RENDER);

        m_renderables.clear();

        Taskflow tf;
        tf.for_each_index(0, static_cast<int>(drawList.size()), 1, [&](int i) {
            RenderableData& data = drawList[i];

            // Cull by distance
            if (m_view->GetPos().Distance(data.position) > m_drawDistance)
                return;

            // Cull by pass mask.
            if (!data.passMask.IsSet(m_passMask))
                return;

            // Cull by frustum
            if (m_view->IsVisible(data.aabb))
            {
                LOCK_GUARD(m_mtx);
                m_renderables.push_back(data);
            }
        });

        JobSystem::Get()->GetMainExecutor().RunAndWait(tf);
    }

    void DrawPass::BatchVisibleRenderables()
    {
        PROFILER_FUNC(PROFILER_THREAD_RENDER);

        m_batches.clear();

        const uint32 size = static_cast<uint32>(m_renderables.size());

        uint32 batchID = 0;
        for (uint32 i = 0; i < size; i++)
        {
            auto r = m_renderables[i];

            for (auto& p : r.meshMaterialPairs)
            {
                int32 index = FindInBatches(p);

                if (index != -1)
                {
                    InstancedBatch& batch = m_batches.at(index);
                    batch.renderableIndices.push_back(i);
                    batch.count++;
                }
                else
                {
                    InstancedBatch newBatch;
                    newBatch.meshAndMaterial.material = p.material;
                    newBatch.meshAndMaterial.mesh     = p.mesh;
                    newBatch.firstInstance            = i;
                    newBatch.renderableIndices.push_back(i);
                    newBatch.count = 1;
                    m_batches.push_back(newBatch);
                }
            }
        }
    }

    void DrawPass::PrepareBuffers()
    {
        PROFILER_FUNC(PROFILER_THREAD_RENDER);

        auto&        renderer   = RenderEngine::Get()->GetLevelRenderer();
        const uint32 frameIndex = renderer.GetFrameIndex();

        // Obj data buffer.
        Vector<GPUObjectData> gpuObjectData;
        for (auto& b : m_batches)
        {
            const uint32 count = static_cast<uint32>(b.renderableIndices.size());

            for (uint32 i = 0; i < count; i++)
            {
                // Object data.
                GPUObjectData objData;
                objData.modelMatrix = m_renderables[b.renderableIndices[i]].modelMatrix;
                gpuObjectData.push_back(objData);
            }
        }

        renderer.GetObjectDataBuffer().CopyInto(gpuObjectData.data(), sizeof(GPUObjectData) * gpuObjectData.size());

        // Scene properties buffer.
        GPUSceneData sceneData = RenderEngine::Get()->GetLevelRenderer().GetSceneData();
        sceneData.proj         = m_view->GetProj();
        sceneData.view         = m_view->GetView();
        sceneData.viewProj     = sceneData.proj * sceneData.view;
        renderer.GetScenePropertiesBuffer().CopyIntoPadded(&sceneData, sizeof(GPUSceneData), VulkanUtility::PadUniformBufferSize(sizeof(GPUSceneData)) * frameIndex);
    }

    void DrawPass::RecordDrawCommands(CommandBuffer& cmd)
    {
        PROFILER_FUNC(PROFILER_THREAD_RENDER);

        PrepareBuffers();

        const uint32 batchesSize = static_cast<uint32>(m_batches.size());
        for (uint32 i = 0; i < batchesSize; i++)
        {
            InstancedBatch& batch = m_batches[i];
            Material*       mat   = batch.meshAndMaterial.material;
            Mesh*           mesh  = batch.meshAndMaterial.mesh;

            auto& pipeline = mat->GetShaderHandle().value->GetPipeline();
            pipeline.Bind(cmd, PipelineBindPoint::Graphics);

            auto&          renderer = RenderEngine::Get()->GetLevelRenderer();
            DescriptorSet& descSet  = renderer.GetGlobalSet();
            DescriptorSet& objSet   = renderer.GetObjectSet();
            DescriptorSet& txtSet   = renderer.GetTextureSet();

            uint32_t uniformOffset = VulkanUtility::PadUniformBufferSize(sizeof(GPUSceneData)) * renderer.GetFrameIndex();

            cmd.CMD_BindDescriptorSets(PipelineBindPoint::Graphics, pipeline._layout, 0, 1, &descSet, 1, &uniformOffset);
            cmd.CMD_BindDescriptorSets(PipelineBindPoint::Graphics, pipeline._layout, 1, 1, &objSet, 0, nullptr);
            // cmd.CMD_BindDescriptorSets(PipelineBindPoint::Graphics, pipeline._layout, 2, 1, &txtSet, 0, nullptr);

            uint64 offset = 0;
            cmd.CMD_BindVertexBuffers(0, 1, mesh->GetGPUVtxBuffer()._ptr, &offset);
            cmd.CMD_BindIndexBuffers(mesh->GetGPUIndexBuffer()._ptr, 0, IndexType::Uint32);
            cmd.CMD_DrawIndexed(static_cast<uint32>(mesh->GetIndexSize()), batch.count, 0, 0, batch.firstInstance);
        }
    }

    int32 DrawPass::FindInBatches(const MeshMaterialPair& pair)
    {
        const int32 size = static_cast<int32>(m_batches.size());

        for (int32 i = 0; i < size; i++)
        {
            const auto& b = m_batches.at(i);
            if (b.meshAndMaterial.mesh == pair.mesh && b.meshAndMaterial.material == pair.material)
                return i;
        }

        return -1;
    }

} // namespace Lina::Graphics
