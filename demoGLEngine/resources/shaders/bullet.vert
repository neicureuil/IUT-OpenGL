#version 330 core

layout(location = 0) in vec3 vertices;
layout(location = 1) in vec3 position; 

uniform mat4 view;
uniform mat4 projection;

void main() {
	vec3 vertex_pos = position + vertices;
	gl_Position = projection * view * vec4(vertex_pos, 1.0f); 
}