#version 330 core
out vec4 FragColor;

in vec2 texCoord;
uniform sampler2D textureTarget;

in float isDeformed;
in vec3 defColor;

void main() {
	vec4 color = vec4(defColor, 1.0);
		
	FragColor = mix(texture(textureTarget, texCoord), color, isDeformed);
}