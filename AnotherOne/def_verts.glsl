#version 460 core

layout(location = 0) in vec3 world_pos;
layout(location = 1) in vec2 i_uv;
layout(location = 2) in vec3 i_normal;

out vec2 uv;
out vec3 norm;

uniform mat4 MVP;

void main()
{

	gl_Position =  MVP * vec4(world_pos,1);
	uv = i_uv;
	norm = i_normal;
}
