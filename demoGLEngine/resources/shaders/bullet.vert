#version 330 core

layout(location = 0) in vec3 vertices;
layout(location = 1) in vec3 position; 
layout(location = 2) in vec3 color; 

uniform mat4 view;
uniform mat4 projection;

out vec3 fcolor;

void main() {
	fcolor = color;
	vec3 vertex_pos = position + vertices;
	gl_Position = projection * view * vec4(vertex_pos, 1.0f); 
}