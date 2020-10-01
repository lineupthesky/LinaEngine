/*
 * Copyright (C) 2019 Inan Evin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#if defined(VS_BUILD)
#include <../UniformBuffers.glh>
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in mat4 model;
layout (location = 6) in mat4 inverseTransposeModel;
out vec2 TexCoords;
out vec3 FragPos;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	FragPos = vec3(model * vec4(position,1.0));
	TexCoords = texCoords;
}

#elif defined(FS_BUILD)
#include <../UniformBuffers.glh>
#include <../Utility.glh>
#include <../MaterialSamplers.glh>
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;
in vec3 FragPos;
in vec2 TexCoords;

struct Material
{
  MaterialSampler2D diffuse;
  vec3 objectColor;
};

uniform Material material;


void main()
{
	fragColor = (material.diffuse.isActive ? texture(material.diffuse.texture ,TexCoords) : vec4(1.0)) * vec4(material.objectColor, 1.0);

}
#endif
