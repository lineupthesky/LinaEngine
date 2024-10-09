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

#include "Core/Graphics/Resource/Shader.hpp"
#include "Core/Application.hpp"
#include "Core/Resources/ResourceManager.hpp"
#include "Core/Graphics/Utility/ShaderPreprocessor.hpp"
#include "Core/Graphics/Utility/GfxHelpers.hpp"
#include "Core/Graphics/Pipeline/DescriptorSet.hpp"

#include "Common/FileSystem//FileSystem.hpp"

namespace Lina
{
	void ShaderProperty::SaveToStream(OStream& stream) const
	{
		stream << sid;
		stream << type;
		stream << name;
		stream << static_cast<uint32>(data.size());
		stream.WriteRaw(data);
	}

	void ShaderProperty::LoadFromStream(IStream& stream)
	{
		stream >> sid;
		stream >> type;
		stream >> name;
		uint32 sz = 0;
		stream >> sz;
		data = {new uint8[static_cast<size_t>(sz)], static_cast<size_t>(sz)};
		stream.ReadToRaw(data);
	}

	void Shader::Metadata::SaveToStream(OStream& stream) const
	{
		stream << variants;
		stream << drawIndirectEnabled;
		stream << shaderType;
	}

	void Shader::Metadata::LoadFromStream(IStream& stream)
	{
		stream >> variants;
		stream >> drawIndirectEnabled;
		stream >> shaderType;
	}

	Shader::~Shader()
	{
		for (ShaderProperty* p : m_properties)
			delete p;
		DestroyHW();
	}

	void Shader::Bind(LinaGX::CommandStream* stream, uint32 gpuHandle)
	{
		LinaGX::CMDBindPipeline* bind = stream->AddCommand<LinaGX::CMDBindPipeline>();
		bind->shader				  = gpuHandle;
	}

	void Shader::AllocateDescriptorSet(DescriptorSet*& outSet, uint32& outIndex)
	{
		/*
		 Return any set that has available allocations.
		 Consumer will use the set pointer and returned allocation index to free up the allocation.
		 */
		for (auto* set : m_descriptorSets)
		{
			if (set->IsAvailable())
			{
				outSet = set;
				set->Allocate(outIndex);
				return;
			}
		}

		// Grow if needed.
		DescriptorSet* set = new DescriptorSet();
		set->Create(m_materialSetDesc);
		m_descriptorSets.push_back(set);
		outSet = set;
		set->Allocate(outIndex);
	}

	void Shader::FreeDescriptorSet(DescriptorSet* set, uint32 index)
	{
		set->Free(index);

		// Shrink if not used anymore.
		if (set->IsEmpty())
		{
			auto it = linatl::find_if(m_descriptorSets.begin(), m_descriptorSets.end(), [set](DescriptorSet* s) -> bool { return s == set; });
			LINA_ASSERT(it != m_descriptorSets.end(), "");
			set->Destroy();
			delete set;
			m_descriptorSets.erase(it);
		}
	}

	bool Shader::LoadFromFile(const String& path)
	{
		if (!FileSystem::FileOrPathExists(path))
			return false;

		const String includePath = FileSystem::GetRunningDirectory();

		if (includePath.empty())
			return false;

		String txt = FileSystem::ReadFileContentsAsString(path);

		if (txt.empty())
			return false;

		LINAGX_MAP<LinaGX::ShaderStage, LinaGX::ShaderCompileData> data;
		LINAGX_MAP<LinaGX::ShaderStage, String>					   blocks;

		HashMap<LinaGX::ShaderStage, String> outStages;

		bool success = ShaderPreprocessor::Preprocess(txt, outStages, m_properties, m_meta.shaderType);
		if (!success)
			return false;

		for (const auto& [stg, text] : outStages)
		{
			LinaGX::ShaderCompileData compData;
			compData.includePath = includePath.c_str();
			compData.text		 = text.c_str();
			data[stg]			 = compData;
		}

		success = LinaGX::Instance::CompileShader(data, m_outCompiledBlobs, m_layout);

		if (!success)
		{
			LINA_ERR("Shader: Failed compiling shader! {0}", m_name);

			for (auto& [stage, blob] : m_outCompiledBlobs)
			{
				if (blob.ptr != nullptr)
					delete[] blob.ptr;
			}

			return false;
		}

		return true;
	}

	void Shader::SaveToStream(OStream& stream) const
	{
		Resource::SaveToStream(stream);
		stream << VERSION;
		stream << m_meta;

		const uint32 size = static_cast<uint32>(m_outCompiledBlobs.size());
		stream << size;

		for (const auto& [stage, blob] : m_outCompiledBlobs)
		{
			stream << stage;
			stream << static_cast<uint32>(blob.size);
			stream.WriteRawEndianSafe(blob.ptr, blob.size);
		}

		SaveLinaGXShaderLayout(stream, m_layout);
		stream << m_properties;
	}

	void Shader::LoadFromStream(IStream& stream)
	{
		Resource::LoadFromStream(stream);
		uint32 version = 0;
		stream >> version;
		stream >> m_meta;

		uint32 size = 0;
		stream >> size;

		for (uint32 i = 0; i < size; i++)
		{
			uint32				sz = 0;
			LinaGX::ShaderStage stage;
			stream >> stage;
			stream >> sz;

			LinaGX::DataBlob blob = {.ptr = nullptr, .size = static_cast<size_t>(sz)};

			if (blob.size != 0)
			{
				blob.ptr = new uint8[blob.size];
				stream.ReadToRawEndianSafe(blob.ptr, blob.size);
			}

			m_outCompiledBlobs[stage] = blob;
		}

		m_layout = {};
		LoadLinaGXShaderLayout(stream, m_layout);
		stream >> m_properties;
	}

	void Shader::GenerateHW()
	{

		/*
		   Create a descriptor set description from the reflection info, this will be used to create descritor sets for the materials using this shader.
			Create a pipeline layout, using global set description, description of the render pass we are using, and the material set description.
		 Materials will use this layout when binding descriptor sets for this shader.
		 */

		if (m_layout.descriptorSetLayouts.size() > 2)
		{
			const auto& setLayout = m_layout.descriptorSetLayouts[2];

			m_materialSetInfo = m_layout.descriptorSetLayouts[2];
			m_materialSetDesc = {};
			for (const auto& b : setLayout.bindings)
				m_materialSetDesc.bindings.push_back(GfxHelpers::GetBindingFromShaderBinding(b));

			m_materialSetDesc.allocationCount = 1;
		}

		m_descriptorSets.push_back(new DescriptorSet());
		m_descriptorSets[0]->Create(m_materialSetDesc);

		// Create variants
		for (auto& [sid, variant] : m_meta.variants)
		{
			LinaGX::ColorBlendAttachment blend = LinaGX::ColorBlendAttachment{
				.blendEnabled		 = !variant.blendDisable,
				.srcColorBlendFactor = variant.blendSrcFactor,
				.dstColorBlendFactor = variant.blendDstFactor,
				.colorBlendOp		 = variant.blendColorOp,
				.srcAlphaBlendFactor = variant.blendSrcAlphaFactor,
				.dstAlphaBlendFactor = variant.blendDstAlphaFactor,
				.alphaBlendOp		 = variant.blendAlphaOp,
				.componentFlags		 = {LinaGX::ColorComponentFlags::R, LinaGX::ColorComponentFlags::G, LinaGX::ColorComponentFlags::B, LinaGX::ColorComponentFlags::A},
			};

			LINAGX_VEC<LinaGX::ShaderColorAttachment> colorAttachments;
			colorAttachments.resize(variant.targets.size());

			for (size_t i = 0; i < variant.targets.size(); i++)
			{
				colorAttachments[i] = {
					.format			 = variant.targets[i].format,
					.blendAttachment = blend,
				};
			}

			LinaGX::ShaderDepthStencilDesc depthStencilAtt = {
				.depthStencilAttachmentFormat = variant.depthFormat,
				.depthWrite					  = variant.depthWrite,
				.depthTest					  = variant.depthTest,
				.depthCompare				  = variant.depthOp,
			};

			variant._gpuHandle = Application::GetLGX()->CreateShader({
				.stages					 = m_outCompiledBlobs,
				.colorAttachments		 = colorAttachments,
				.depthStencilDesc		 = depthStencilAtt,
				.layout					 = m_layout,
				.polygonMode			 = LinaGX::PolygonMode::Fill,
				.cullMode				 = variant.cullMode,
				.frontFace				 = variant.frontFace,
				.topology				 = variant.topology,
				.depthBiasEnable		 = variant.depthBiasEnable,
				.depthBiasConstant		 = variant.depthBiasConstant,
				.depthBiasClamp			 = variant.depthBiasClamp,
				.depthBiasSlope			 = variant.depthBiasSlope,
				.drawIndirectEnabled	 = m_meta.drawIndirectEnabled,
				.useCustomPipelineLayout = false,
				.debugName				 = m_name.c_str(),
			});

			variant._gpuHandleExists = true;
		}

		for (auto& [stage, blob] : m_outCompiledBlobs)
		{
			delete[] blob.ptr;
		}
	}

	void Shader::DestroyHW()
	{
		for (auto& [sid, var] : m_meta.variants)
		{
			if (!var._gpuHandleExists)
				continue;

			var._gpuHandleExists = false;
			Application::GetLGX()->DestroyShader(var._gpuHandle);
		}

		for (const auto& d : m_descriptorSets)
		{
			d->Destroy();
			delete d;
		}
	}

	Vector<ShaderProperty*> Shader::CopyProperties()
	{
		Vector<ShaderProperty*> props;

		for (ShaderProperty* property : m_properties)
		{
			ShaderProperty* copy = new ShaderProperty();
			copy->name			 = property->name;
			copy->sid			 = property->sid;
			copy->type			 = property->type;
			copy->data			 = {new uint8[property->data.size()], property->data.size()};
			MEMCPY(copy->data.data(), property->data.data(), property->data.size());
			props.push_back(copy);
		}

		return props;
	}
} // namespace Lina
