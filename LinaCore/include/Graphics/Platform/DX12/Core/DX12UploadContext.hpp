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

#ifndef DX12UploadContext_HPP
#define DX12UploadContext_HPP

#include "Graphics/Core/IUploadContext.hpp"
#include "Graphics/Platform/DX12/Core/DX12Common.hpp"

namespace Lina
{
	class Renderer;

	class DX12UploadContext : public IUploadContext
	{
	public:
		DX12UploadContext(Renderer* rend);
		virtual ~DX12UploadContext();

		virtual void Flush(uint32 frameIndex, Bitmask16 flushFlags) override;
		virtual void UploadBuffers(IGfxBufferResource* targetGPUResource, void* data, size_t dataSize) override;
		virtual void UploadTexture(IGfxTextureResource* targetGPUTexture, Texture* src, ImageGenerateRequest req = {}) override;
		virtual void UploadBuffersImmediate(IGfxBufferResource* targetGpuResource, IGfxBufferResource* staging) override;
		virtual void PushCustomCommand(const GfxCommand& cmd) override;

	private:
		Microsoft::WRL::ComPtr<ID3D12Fence>				   m_fence;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> m_cmdLists[FRAMES_IN_FLIGHT];
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>	   m_cmdAllocator;
		HANDLE											   m_fenceEvent = NULL;
		uint64											   m_fenceValue = 0;
	};
} // namespace Lina

#endif