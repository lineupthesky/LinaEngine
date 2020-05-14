/*
 * Copyright (C) 2019 Inan Evin - Thanks to the contributions of Benny Bobaganoosh
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
 
#include "common.glh"

varying vec2 texCoord0;

#if defined(VS_BUILD)
Layout(0) attribute vec3 position;
Layout(1) attribute vec2 texCoord;
Layout(4) attribute mat4 model;


layout (std140, row_major) uniform GlobalMatrices
{
	mat4 projection;
	mat4 view;
};

out vec2 TexCoords;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
	FragPos = vec3(model * vec4(position,1.0));
    TexCoords = texCoord;
}

#elif defined(FS_BUILD)


struct Material
{
sampler2D diffuse;
};

uniform Material material;

in vec3 FragPos;
in vec2 TexCoords;
out vec4 fragColor;

void main()
{
	fragColor = texture(material.diffuse, TexCoords);
}
#endif