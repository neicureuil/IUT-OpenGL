#version 330 core

in vec2 UV;
in vec4 particlecolor;

uniform sampler2D textureParticle;

out vec4 FragColor;

void main(){
	FragColor = texture(textureParticle, UV);
}