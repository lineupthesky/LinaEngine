/*
Author: Inan Evin
www.inanevin.com

Copyright 2018 Inan Evin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions 
and limitations under the License.

Class: GameRenderContext
Timestamp: 4/27/2019 5:53:43 PM

*/

#pragma once

#ifndef GameRenderContext_HPP
#define GameRenderContext_HPP

#include "RenderContext.hpp"

namespace LinaEngine::Graphics
{
	class GameRenderContext : public RenderContext
	{
	public:
		GameRenderContext(RenderEngine<PAMRenderEngine>& deviceIn, RenderTarget& targetIn, DrawParams& drawParamsIn, Shader& shaderIn, Sampler& samplerIn, const Matrix4F& perspectiveIn)
			: RenderContext(deviceIn, targetIn), drawParams(drawParamsIn), shader(shaderIn), sampler(samplerIn), perspective(perspectiveIn) {};

		FORCEINLINE void RenderMesh(VertexArray& vertexArray, Texture& texture, const Matrix4F& transformIn)
		{
			meshRenderBuffer[LinaMakePair(&vertexArray, &texture)].push_back(perspective * transformIn);
		}

		void Flush();

	private:

		DrawParams& drawParams;
		Shader& shader;
		Sampler& sampler;
		Matrix4F perspective;
		LinaMap<LinaPair<VertexArray*, Texture*>, LinaArray<Matrix4F> > meshRenderBuffer;

	};
}


#endif