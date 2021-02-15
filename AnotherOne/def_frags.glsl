#version 460 core

in vec2 uv;
in vec3 norm;

out vec4 color;

void main()
{
	color = vec4(uv, 1.0, 1.0);
}