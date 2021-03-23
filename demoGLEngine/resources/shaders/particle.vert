#version 330 core

layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; 
layout(location = 2) in vec2 life; 

out vec2 UV;

uniform vec3 camRight;
uniform vec3 camUp;

uniform mat4 view; 
uniform mat4 projection; 

// Nombre de frame en x et en y sur la texture
uniform vec2 atlasSize;

void main() {
	// Billboard
	float particleSize = xyzs.w; 
	vec3 center = xyzs.xyz;
	
	vec3 vetexPos = center + camRight * squareVertices.x * particleSize + camUp * squareVertices.y * particleSize;
	gl_Position = projection * view * vec4(vetexPos, 1.0f);

	//Animation de la texture (choix de l'offset dans l'atlas)
	int texAtlasNum = int(ceil((life.x / life.y) * atlasSize.x * atlasSize.y));
	vec2 offset = vec2(float(texAtlasNum%int(atlasSize.x)), floor(float(texAtlasNum)/atlasSize.y));
	UV = (squareVertices.xy  + vec2(0.5, 0.5) + offset) / atlasSize;
}