#version 330 core

layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; 
layout(location = 2) in vec2 life; 

out vec2 UV;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; 

void main() {
	// Billboard
	float particleSize = xyzs.w; 
	vec3 particleCenter_wordspace = xyzs.xyz;
	
	vec3 vertexPosition_worldspace = particleCenter_wordspace + CameraRight_worldspace * squareVertices.x * particleSize + CameraUp_worldspace * squareVertices.y * particleSize;
	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);

	//Animation de la texture (choix de l'offset dans l'atlas)
	int texAtlasNum = int(ceil((life.x / life.y) * 64.0f));
	vec2 offset = vec2(float(texAtlasNum%8), floor(float(texAtlasNum)/8.0f));
	UV = (squareVertices.xy  + vec2(0.5, 0.5) + offset) / 8;
}