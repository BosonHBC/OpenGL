#version 420
layout (location = 0) in vec3 pos;

layout(std140, binding = 1) uniform uniformBuffer_drawcall
{
	mat4 modelMatrix;
	mat4 normalMatrix;
};
uniform mat4 directionalLightTransform; // projection(ortho or perpective) * view

void main()
{
	gl_Position = directionalLightTransform * modelMatrix * vec4(pos, 1.0);
}