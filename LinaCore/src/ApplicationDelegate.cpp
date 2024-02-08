#include "Core/ApplicationDelegate.hpp"
#include "Core/Resources/ResourceManager.hpp"
#include "Core/Graphics/Resource/Model.hpp"
#include "Core/Graphics/Resource/Texture.hpp"
#include "Core/Graphics/Resource/TextureSampler.hpp"
#include "Core/Graphics/Resource/Font.hpp"
#include "Core/Graphics/Resource/Shader.hpp"
#include "Core/Graphics/Resource/Material.hpp"

namespace Lina
{
	void ApplicationDelegate::RegisterResourceTypes(ResourceManager& rm)
	{
		rm.RegisterResourceType<Model>(50, {"glb"}, PackageType::Package1);
		rm.RegisterResourceType<Shader>(25, {"linashader"}, PackageType::Package1);
		rm.RegisterResourceType<Texture>(100, {"png", "jpeg", "jpg"}, PackageType::Package1);
		rm.RegisterResourceType<TextureSampler>(100, {"linasampler"}, PackageType::Package1);
		rm.RegisterResourceType<Font>(10, {"ttf", "otf"}, PackageType::Package1);
		rm.RegisterResourceType<Material>(25, {"linamaterial"}, PackageType::Package1);
	}

	Vector<ResourceIdentifier> ApplicationDelegate::GetPriorityResources()
	{
		Vector<ResourceIdentifier> list;

		list.push_back(ResourceIdentifier("Resources/Core/Fonts/NunitoSans_1x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/NunitoSans_2x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/NunitoSans_3x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/NunitoSans_4x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/Rubik-Regular_1x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/Rubik-Regular_2x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/Rubik-Regular_3x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/Rubik-Regular_4x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Textures/StubBlack.png", GetTypeID<Texture>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Shaders/Test.linashader", GetTypeID<Shader>(), 0));
		// list.push_back(ResourceIdentifier("Resources/Core/Shaders/UnlitStandard.linashader", GetTypeID<Shader>(), 0));
		// list.push_back(ResourceIdentifier("Resources/Core/Shaders/GUIStandard.linashader", GetTypeID<Shader>(), 0));
		// list.push_back(ResourceIdentifier("Resources/Core/Shaders/ScreenQuads/SQTexture.linashader", GetTypeID<Shader>(), 0));

		for (auto& ident : list)
			ident.sid = TO_SID(ident.path);

		return list;
	}

	Vector<ResourceIdentifier> ApplicationDelegate::GetCoreResources()
	{
		Vector<ResourceIdentifier> list;

		list.push_back(ResourceIdentifier("Resources/Core/Textures/StubLinaLogo.png", GetTypeID<Texture>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Textures/StubLinaLogoWhite.png", GetTypeID<Texture>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Textures/StubLinaLogoText.png", GetTypeID<Texture>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/WorkSans-Regular_1x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/WorkSans-Regular_2x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/WorkSans-Regular_3x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/WorkSans-Regular_4x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/NunitoSans_Bold_1x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/NunitoSans_Bold_2x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/NunitoSans_Bold_3x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Fonts/NunitoSans_Bold_4x.ttf", GetTypeID<Font>(), 0));
		list.push_back(ResourceIdentifier("Resources/Core/Models/LinaLogo.glb", GetTypeID<Model>(), 0));

		for (auto& ident : list)
			ident.sid = TO_SID(ident.path);

		return list;
	}

	bool ApplicationDelegate::FillResourceCustomMeta(StringID sid, OStream& stream)
	{
		if (sid == "Resources/Core/Fonts/NunitoSans_1x.ttf"_hs)
		{
			Font::Metadata customMeta = {
				.pointSize	 = 12,
				.isSDF		 = false,
				.glyphRanges = {linatl::make_pair(160, 360)},
			};
			customMeta.SaveToStream(stream);
			return true;
		}

		if (sid == "Resources/Core/Fonts/NunitoSans_2x.ttf"_hs)
		{
			Font::Metadata customMeta = {
				.pointSize	 = 14,
				.isSDF		 = false,
				.glyphRanges = {linatl::make_pair(160, 360)},
			};
			customMeta.SaveToStream(stream);
			return true;
		}

		if (sid == "Resources/Core/Fonts/NunitoSans_3x.ttf"_hs)
		{
			Font::Metadata customMeta = {
				.pointSize	 = 16,
				.isSDF		 = false,
				.glyphRanges = {linatl::make_pair(160, 360)},
			};
			customMeta.SaveToStream(stream);
			return true;
		}

		if (sid == "Resources/Core/Fonts/NunitoSans_4x.ttf"_hs)
		{
			Font::Metadata customMeta = {
				.pointSize	 = 20,
				.isSDF		 = false,
				.glyphRanges = {linatl::make_pair(160, 360)},
			};
			customMeta.SaveToStream(stream);
			return true;
		}

		if (sid == "Resources/Core/Fonts/Rubik-Regular_1x.ttf"_hs)
		{
			Font::Metadata customMeta = {
				.pointSize = 12,
				.isSDF	   = false,
			};
			customMeta.SaveToStream(stream);
			return true;
		}

		if (sid == "Resources/Core/Fonts/Rubik-Regular_2x.ttf"_hs)
		{
			Font::Metadata customMeta = {
				.pointSize = 14,
				.isSDF	   = false,
			};
			customMeta.SaveToStream(stream);
			return true;
		}

		if (sid == "Resources/Core/Fonts/Rubik-Regular_3x.ttf"_hs)
		{
			Font::Metadata customMeta = {
				.pointSize = 16,
				.isSDF	   = false,
			};
			customMeta.SaveToStream(stream);
			return true;
		}

		if (sid == "Resources/Core/Fonts/Rubik-Regular_4x.ttf"_hs)
		{
			Font::Metadata customMeta = {
				.pointSize = 18,
				.isSDF	   = false,
			};
			customMeta.SaveToStream(stream);
			return true;
		}

		if (sid == "Resources/Core/Fonts/Rubik-Regular_4x.ttf"_hs)
		{
			Font::Metadata customMeta = {
				.pointSize = 18,
				.isSDF	   = false,
			};
			customMeta.SaveToStream(stream);
			return true;
		}

		// NOTE: 160, 380 is the glyph range for nunito sans

		if (sid == "Resources/Core/Textures/StubLinaLogo.png"_hs || "Resources/Core/Textures/StubLinaLogoWhite.png"_hs || "Resources/Core/Textures/StubLinaLogoText.png"_hs)
		{
			Texture::Metadata meta = {
				.samplerSID = DEFAULT_GUI_SAMPLER_SID,
			};

			meta.SaveToStream(stream);
			return true;
		}

		if (sid == "Resources/Core/Shaders/Test.linashader"_hs)
		{
			Shader::Metadata meta;

			meta.variants["Default"_hs] = ShaderVariant{
				.targetType	  = ShaderWriteTargetType::RenderTarget,
				.cullMode	  = LinaGX::CullMode::None,
				.frontFace	  = LinaGX::FrontFace::CCW,
				.depthDisable = true,
				.blendDisable = true,
			};

			meta.variants["Final"_hs] = ShaderVariant{
				.targetType	  = ShaderWriteTargetType::Swapchain,
				.cullMode	  = LinaGX::CullMode::None,
				.frontFace	  = LinaGX::FrontFace::CCW,
				.depthDisable = true,
				.blendDisable = true,
			};

			meta.SaveToStream(stream);
			return true;
		}

		if (sid == DEFAULT_SHADER_GUI)
		{
			Shader::Metadata meta;

			// variants.

			meta.descriptorSetAllocationCount = 50;
			meta.SaveToStream(stream);
		}

		return false;
	}

} // namespace Lina