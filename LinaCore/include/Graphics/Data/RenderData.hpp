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
#include "Data/Bitmask.hpp"

namespace Lina
{
	class RenderableComponent;
	class Mesh;
	class Material;

#define OBJ_BUFFER_MAX	 15
#define MAX_LIGHTS		 10
#define FRAMES_IN_FLIGHT 2

	enum RenderableType
	{
		RenderableSprite	  = 1 << 0,
		RenderableDecal		  = 1 << 1,
		RenderableSky		  = 1 << 2,
		RenderableSkinnedMesh = 1 << 3,
		RenderableStaticMesh  = 1 << 4,
		RenderableParticle	  = 1 << 5,
	};

	enum DrawPassMask
	{
		Opaque		= 1 << 0,
		Transparent = 1 << 1,
		Shadow		= 1 << 2,
	};

	enum SurfaceRendererMask
	{
		SRM_RenderGUI			 = 1 << 0,
		SRM_DrawOffscreenTexture = 1 << 1,
	};

	enum WorldRendererMask
	{
		WRM_PassResultToSurface = 1 << 0,
		WRM_ApplyPostProcessing = 1 << 1,
	};

	enum class RendererType
	{
		None,
		WorldRenderer,
		SurfaceRenderer
	};

	struct VisibilityData
	{
		RenderableComponent* renderable = nullptr;
		Vector3				 position	= Vector3::Zero;
		AABB				 aabb		= AABB();
		bool				 valid		= false;
	};

	struct TestData
	{
		uint32 id = 0;
	};

	struct MeshMaterialPair
	{
		Mesh*	  mesh	   = nullptr;
		Material* material = nullptr;
	};
	struct InstancedBatch
	{
		Material*	   mat = nullptr;
		Vector<Mesh*>  meshes;
		Vector<uint32> renderableIndices;
		uint32		   firstInstance = 0;
		uint32		   count		 = 0;
	};

	struct RenderableData
	{
		RenderableType			 type		  = RenderableType::RenderableDecal;
		Bitmask16				 passMask	  = DrawPassMask::Opaque;
		Matrix4					 modelMatrix  = Matrix4::Identity();
		Vector3					 position	  = Vector3();
		AABB					 aabb		  = AABB();
		uint32					 entityID	  = 0;
		uint32					 batchID	  = 0;
		uint32					 objDataIndex = 0;
		Bitmask16				 entityMask	  = 0;
		Vector<MeshMaterialPair> meshMaterialPairs;
	};

	struct GPUSceneData
	{
		Vector4 fogColor		  = Vector4::Zero; // w is for exponent
		Vector4 fogDistances	  = Vector4::Zero; // x for min, y for max, zw unused.
		Vector4 ambientColor	  = Vector4::Zero;
		Vector4 sunlightDirection = Vector4::Zero; // w for sun power
		Vector4 sunlightColor	  = Vector4::Zero;
	};

	struct GPUGlobalData
	{
		Vector4 screenSizeMousePos = Vector4::Zero;
		Vector2 deltaElapsed	   = Vector2::Zero;
	};

	struct GPUDebugData
	{
		int visualizeDepth = 0;
	};

	struct GPUObjectData
	{
		Matrix4 modelMatrix = Matrix4::Identity();
	};

	struct GPUViewData
	{
		Matrix4 view	   = Matrix4::Identity();
		Matrix4 proj	   = Matrix4::Identity();
		Matrix4 viewProj   = Matrix4::Identity();
		Vector4 camPos	   = Vector4::Zero;
		Vector2 camNearFar = Vector2::Zero;
	};

	struct LightData
	{
		Vector4 position = Vector4::Zero;
	};

	struct GPULightData
	{
		LightData lights[MAX_LIGHTS];
	};

	struct MergedBufferMeshEntry
	{
		uint32 vertexOffset = 0;
		uint32 indexSize	= 0;
		uint32 firstIndex	= 0;
	};

	struct PCRTest
	{
		Matrix4 mat = Matrix4::Identity();
	};

} // namespace Lina

#endif