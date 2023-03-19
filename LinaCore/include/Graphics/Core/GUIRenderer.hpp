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

#ifndef GUIRenderer_HPP
#define GUIRenderer_HPP

#include "Graphics/Data/RenderData.hpp"
#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"

namespace LinaVG
{
	struct GradientDrawBuffer;
	struct TextureDrawBuffer;
	struct DrawBuffer;
	struct SimpleTextDrawBuffer;
	struct SDFTextDrawBuffer;
	struct DrawBuffer;

} // namespace LinaVG

namespace Lina
{
	class GfxManager;
	class Renderer;
	class Material;
	class IGfxBufferResource;

	class GUIRenderer
	{

	private:
		enum class LinaVGDrawCategoryType
		{
			Default,
			Gradient,
			Textured,
			SimpleText,
			SDF
		};

		struct GUIStandardMaterialDefinition
		{
			Vector4	 color1		  = Vector4::Zero;
			Vector4	 color2		  = Vector4::Zero;
			Vector4	 float4pack1  = Vector4::Zero;
			Vector4	 float4pack2  = Vector4::Zero;
			Vector4i intpack1	  = Vector4i::Zero;
			StringID diffuseIndex = 0;
		};

		struct OrderedDrawRequestMeta
		{
			uint32 clipX = 0;
			uint32 clipY = 0;
			uint32 clipW = 0;
			uint32 clipH = 0;
		};

		struct OrderedDrawRequest
		{
			uint32						  vertexOffset = 0;
			uint32						  firstIndex   = 0;
			uint32						  indexSize	   = 0;
			uint32						  matId		   = 0;
			GUIStandardMaterialDefinition materialDefinition;
			LinaVGDrawCategoryType		  type = LinaVGDrawCategoryType::Default;
			OrderedDrawRequestMeta		  meta;
		};

		struct DataPerFrame
		{
			IGfxBufferResource*		   vtxBufferStaging	  = nullptr;
			IGfxBufferResource*		   vtxBufferGPU		  = nullptr;
			IGfxBufferResource*		   indexBufferStaging = nullptr;
			IGfxBufferResource*		   indexBufferGPU	  = nullptr;
			IGfxBufferResource*		   viewDataBuffer	  = nullptr;
			GPUViewData				   viewData;
			Vector<OrderedDrawRequest> drawRequests;
			uint32					   indexCounter	 = 0;
			uint32					   vertexCounter = 0;
		};

		struct DataPerImage
		{
			Material* guiMaterial = nullptr;
		};

	public:
		GUIRenderer(GfxManager* gfxMan, StringID ownerSid, uint32 imageCount);
		virtual ~GUIRenderer();

		void FeedGradient(LinaVG::GradientDrawBuffer* buf);
		void FeedTextured(LinaVG::TextureDrawBuffer* buf);
		void FeedDefault(LinaVG::DrawBuffer* buf);
		void FeedSimpleText(LinaVG::SimpleTextDrawBuffer* buf);
		void FeedSDFText(LinaVG::SDFTextDrawBuffer* buf);
		void Render(uint32 cmdList);
		void Prepare(const Vector2i& surfaceRendererSize, uint32 frameIndex, uint32 imageIndex);

	private:
		void UpdateProjection();
		void AssignStandardMaterial(Material* mat, const GUIStandardMaterialDefinition& def);

	private:
		OrderedDrawRequest& AddOrderedDrawRequest(LinaVG::DrawBuffer* buf, LinaVGDrawCategoryType type);

	private:
		Vector2i			 m_size		  = Vector2i::Zero;
		uint32				 m_imageIndex = 0;
		uint32				 m_frameIndex = 0;
		StringID			 m_ownerSid	  = 0;
		uint32				 m_imageCount = 0;
		GfxManager*			 m_gfxManager = nullptr;
		Renderer*			 m_renderer	  = nullptr;
		DataPerFrame		 m_frames[FRAMES_IN_FLIGHT];
		Vector<DataPerImage> m_dataPerImage;
		Matrix4				 m_projection;
	};
} // namespace Lina

#endif