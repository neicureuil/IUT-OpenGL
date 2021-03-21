#version 330 core
out vec4 FragColor;

in vec2 texCoord;
uniform sampler2D textureTarget;

in float isDeformed;

void main() {
	vec4 color = vec4(1.0,0.0,0.0, 0.0);

	if(isDeformed >= 1.0) {
		color.a = 1.0;
	}
		
	FragColor = mix(texture(textureTarget, texCoord), color, isDeformed);
	
}