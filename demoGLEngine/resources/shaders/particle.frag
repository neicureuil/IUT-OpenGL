#version 330 core

in vec2 UV;

uniform sampler2D textureParticle;

out vec4 FragColor;

void main(){
	FragColor = texture(textureParticle, UV);
}