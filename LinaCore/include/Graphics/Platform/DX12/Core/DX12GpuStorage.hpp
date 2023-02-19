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

#ifndef D3D12GpuStorage_HPP
#define D3D12GpuStorage_HPP

#include "Data/IDList.hpp"
#include "Graphics/Core/IGpuStorage.hpp"
#include "Graphics/Platform/DX12/Utility/ID3DIncludeInterface.hpp"
#include "Graphics/Platform/DX12/SDK/d3d12.h"
#include <wrl/client.h>

namespace Lina
{

	struct GeneratedTexture
	{
	};

	struct GeneratedShader
	{
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pso;
	};

	struct GeneratedMaterial
	{
	};

	class DX12GfxManager;

	class DX12GpuStorage : public IGpuStorage
	{
	public:
#define DEFAULT_TXT_POOL	20
#define DEFAULT_SHADER_POOL 10
#define DEFAULT_MAT_POOL	20

		DX12GpuStorage(DX12GfxManager* manager) : IGpuStorage(), m_gfxManager(manager), m_textures(DEFAULT_TXT_POOL, GeneratedTexture()), m_shaders(DEFAULT_SHADER_POOL, GeneratedShader()), m_materials(DEFAULT_MAT_POOL, GeneratedMaterial()){};
		virtual ~DX12GpuStorage() = default;

		virtual void Initilalize() override;
		virtual void Shutdown() override;

		virtual uint32 GenerateMaterial(Material* mat, uint32 existingHandle) override;
		virtual void   UpdateMaterialProperties(Material* mat, uint32 imageIndex) override;
		virtual void   UpdateMaterialTextures(Material* mat, uint32 imageIndex, const Vector<uint32>& dirtyTextures) override;
		virtual void   DestroyMaterial(uint32 handle) override;

		virtual uint32 GeneratePipeline(Shader* shader) override;
		virtual void   DestroyPipeline(uint32 handle) override;
		virtual void   CompileShader(const char* path, const HashMap<ShaderStage, String>& stages, HashMap<ShaderStage, Vector<unsigned int>>& outCompiledCode);

		virtual uint32 GenerateImage(Texture* txt, uint32 aspectFlags, uint32 imageUsageFlags) override;
		virtual uint32 GenerateImageAndUpload(Texture* txt) override;
		virtual void   DestroyImage(uint32 handle) override;

	private:
		DX12GfxManager*		  m_gfxManager = nullptr;
		IDList<GeneratedTexture>  m_textures;
		Mutex					  m_textureMtx;
		IDList<GeneratedShader>	  m_shaders;
		Mutex					  m_shaderMtx;
		IDList<GeneratedMaterial> m_materials;
		Mutex					  m_materialMtx;

		Microsoft::WRL::ComPtr<IDxcLibrary>	 m_idxcLib;
		Microsoft::WRL::ComPtr<IDxcCompiler> m_idxcCompiler;
		ID3DIncludeInterface				 m_includeInterface;
	};
} // namespace Lina

#endif