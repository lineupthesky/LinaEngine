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

#include "Editor/Graphics/MousePickRenderer.hpp"
#include "Editor/CommonEditor.hpp"
#include "Editor/Editor.hpp"
#include "Editor/Graphics/EditorGfxHelpers.hpp"
#include "Core/Resources/ResourceManager.hpp"
#include "Core/Graphics/Resource/Shader.hpp"
#include "Core/Graphics/Resource/Material.hpp"
#include "Core/Application.hpp"
#include "Core/Graphics/Pipeline/RenderPass.hpp"
#include "Core/Graphics/Renderers/DrawCollector.hpp"
#include "Core/Graphics/Resource/Texture.hpp"
#include "Core/Graphics/Pipeline/View.hpp"
#include "Core/Graphics/Renderers/WorldRenderer.hpp"
#include "Core/Graphics/Utility/GfxHelpers.hpp"

namespace Lina::Editor
{

#ifdef LINA_DEBUG
#define DEBUG_LABEL_BEGIN(Stream, LABEL)                                                                                                                                                                                                                           \
	{                                                                                                                                                                                                                                                              \
		LinaGX::CMDDebugBeginLabel* debug = Stream->AddCommand<LinaGX::CMDDebugBeginLabel>();                                                                                                                                                                      \
		debug->label					  = LABEL;                                                                                                                                                                                                                 \
	}
#define DEBUG_LABEL_END(Stream)                                                                                                                                                                                                                                    \
	{                                                                                                                                                                                                                                                              \
		Stream->AddCommand<LinaGX::CMDDebugEndLabel>();                                                                                                                                                                                                            \
	}
#else
#define DEBUG_LABEL_BEGIN(Stream, LABEL)
#define DEBUG_LABEL_END(Stream)
#endif

	MousePickRenderer::MousePickRenderer(Editor* editor, LinaGX::Instance* lgx, EntityWorld* world, WorldRenderer* wr, ResourceManagerV2* rm) : FeatureRenderer(lgx, world, rm, wr)
	{
		m_editor	 = editor;
		m_gfxContext = &m_editor->GetApp()->GetGfxContext();
		m_shader	 = m_rm->GetResource<Shader>(EDITOR_SHADER_WORLD_GRID_ID);
		// m_editor->GetApp()->GetGfxContext().MarkBindlessDirty();
		m_pipelineLayout = m_lgx->CreatePipelineLayout(EditorGfxHelpers::GetPipelineLayoutDescriptionEntityBufferPass());

		m_entityBufferPass.Create(EditorGfxHelpers::GetEntityBufferPassDescription());

		for (uint32 i = 0; i < FRAMES_IN_FLIGHT; i++)
		{

			const uint16 set = m_entityBufferPass.GetDescriptorSet(i);

			m_lgx->DescriptorUpdateBuffer({
				.setHandle = set,
				.binding   = 1,
				.buffers   = {m_wr->GetInstanceDataBuffer(i).GetGPUResource()},
			});

			m_lgx->DescriptorUpdateBuffer({
				.setHandle = set,
				.binding   = 2,
				.buffers   = {m_wr->GetEntityDataBuffer(i).GetGPUResource()},
			});

			m_lgx->DescriptorUpdateBuffer({
				.setHandle = set,
				.binding   = 3,
				.buffers   = {m_wr->GetBoneBuffer(i).GetGPUResource()},
			});
		}
	} // namespace Lina::Editor

	MousePickRenderer::~MousePickRenderer()
	{
		DestroySizeRelativeResources();
		m_entityBufferPass.Destroy();
		m_lgx->DestroyPipelineLayout(m_pipelineLayout);
		// m_rm->DestroyResource(m_gridMaterial);
		// m_editor->GetApp()->GetGfxContext().MarkBindlessDirty();
	}

	void MousePickRenderer::DestroySizeRelativeResources()
	{
		for (uint32 i = 0; i < FRAMES_IN_FLIGHT; i++)
		{
			PerFrameData& data = m_pfd[i];

			if (data.depthTarget != nullptr)
			{
				data.depthTarget->DestroyHW();
				m_rm->DestroyResource(data.depthTarget);
				data.depthTarget = nullptr;
			}

			if (data.renderTarget != nullptr)
			{

				data.renderTarget->DestroyHW();
				m_rm->DestroyResource(data.renderTarget);
				data.renderTarget = nullptr;
			}
		}
	}

	void MousePickRenderer::CreateSizeRelativeResources(const Vector2ui& size)
	{
		m_size							 = size;
		const LinaGX::TextureDesc rtDesc = {
			.format = DEFAULT_RT_FORMAT,
			.flags	= LinaGX::TF_ColorAttachment | LinaGX::TF_Sampled,
			.width	= size.x,
			.height = size.y,
		};

		const LinaGX::TextureDesc depthDesc = {
			.format					  = LinaGX::Format::D32_SFLOAT,
			.depthStencilSampleFormat = LinaGX::Format::R32_SFLOAT,
			.flags					  = LinaGX::TF_DepthTexture | LinaGX::TF_Sampled,
			.width					  = m_size.x,
			.height					  = m_size.y,
		};

		for (uint32 i = 0; i < FRAMES_IN_FLIGHT; i++)
		{
			PerFrameData& data = m_pfd[i];

			data.depthTarget  = m_rm->CreateResource<Texture>(m_rm->ConsumeResourceID(), "MousePickDepthTarget");
			data.renderTarget = m_rm->CreateResource<Texture>(m_rm->ConsumeResourceID(), "MousePickRenderTarget");
			data.renderTarget->GenerateHWFromDesc(rtDesc);
			data.depthTarget->GenerateHWFromDesc(depthDesc);
			m_entityBufferPass.SetColorAttachment(i, 0, {.clearColor = {0.0f, 0.0f, 0.0f, 1.0f}, .texture = data.renderTarget->GetGPUHandle(), .isSwapchain = false});
			m_entityBufferPass.DepthStencilAttachment(i, {.useDepth = true, .texture = data.depthTarget->GetGPUHandle(), .depthLoadOp = LinaGX::LoadOp::Clear, .depthStoreOp = LinaGX::StoreOp::Store, .clearDepth = 1.0f});
		}
	}

	void MousePickRenderer::AddBuffersToUploadQueue(uint32 frameIndex, ResourceUploadQueue& queue)
	{
		// View data.
		{
			Camera&		worldCam = m_world->GetWorldCamera();
			GPUDataView view	 = {
					.view = worldCam.GetView(),
					.proj = worldCam.GetProjection(),
			};

			const Vector3& camPos	   = worldCam.GetPosition();
			const Vector3& camDir	   = worldCam.GetRotation().GetForward();
			view.viewProj			   = view.proj * view.view;
			view.cameraPositionAndNear = Vector4(camPos.x, camPos.y, camPos.z, worldCam.GetZNear());
			view.cameraDirectionAndFar = Vector4(camDir.x, camDir.y, camDir.z, worldCam.GetZFar());
			view.size				   = Vector2(static_cast<float>(m_size.x), static_cast<float>(m_size.y));
			m_entityBufferPass.GetBuffer(frameIndex, "ViewData"_hs).BufferData(0, (uint8*)&view, sizeof(GPUDataView));
		}
	}

	void MousePickRenderer::ProduceFrame(DrawCollector& collector)
	{
		collector.CreateGroup("EntityIDs"_hs);

		View cameraView = {};
		cameraView.SetView(m_world->GetWorldCamera().GetView());
		collector.CollectCompModels(collector.GetGroup("EntityIDs"_hs), cameraView, "StaticEntityID"_hs, "SkinnedEntityID"_hs, ShaderType::DeferredSurface);
		collector.CollectCompModels(collector.GetGroup("EntityIDs"_hs), cameraView, "StaticEntityID"_hs, "SkinnedEntityID"_hs, ShaderType::ForwardSurface);
	}

	void MousePickRenderer::PostRender(uint32 frameIndex, LinaGX::CommandStream* stream, const LinaGX::Viewport& vp, const LinaGX::ScissorsRect& sc)
	{
		DrawCollector& drawCollector = m_wr->GetDrawCollector();

		// FORWARD PASS
		{
			DEBUG_LABEL_BEGIN(stream, "Editor: Entity Buffer Pass");

			m_entityBufferPass.Begin(stream, vp, sc, frameIndex);
			m_entityBufferPass.BindDescriptors(stream, frameIndex, m_pipelineLayout, 1);

			drawCollector.RenderGroup("EntityIDs"_hs, stream);

			m_entityBufferPass.End(stream);

			LinaGX::CMDBarrier* barrier	 = stream->AddCommand<LinaGX::CMDBarrier>();
			barrier->srcStageFlags		 = LinaGX::PSF_ColorAttachment | LinaGX::PSF_EarlyFragment;
			barrier->dstStageFlags		 = LinaGX::PSF_FragmentShader;
			barrier->textureBarrierCount = 1;
			barrier->textureBarriers	 = stream->EmplaceAuxMemorySizeOnly<LinaGX::TextureBarrier>(sizeof(LinaGX::TextureBarrier) * 1);
			barrier->textureBarriers[0]	 = GfxHelpers::GetTextureBarrierColorAtt2Read(m_pfd[frameIndex].renderTarget->GetGPUHandle());

			DEBUG_LABEL_END(stream);
		}
	}

	void MousePickRenderer::GetBarriersTextureToAttachment(uint32 frameIndex, Vector<LinaGX::TextureBarrier>& outBarriers)
	{
		outBarriers.push_back(GfxHelpers::GetTextureBarrierColorRead2Att(m_pfd[frameIndex].renderTarget->GetGPUHandle()));
		outBarriers.push_back(GfxHelpers::GetTextureBarrierDepthRead2Att(m_pfd[frameIndex].depthTarget->GetGPUHandle()));
	}

} // namespace Lina::Editor