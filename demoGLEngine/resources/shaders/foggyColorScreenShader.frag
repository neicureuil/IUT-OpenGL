#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D screenDepth;


struct ViewPort {
	float near;
	float far;
};

struct FogParam {
	vec3 color;
	float density;
};

uniform ViewPort viewPort;
uniform FogParam fog;

float LinearizeDepth(float depth, float near, float far) {
    float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    vec4 scenceColor = vec4(texture(screenTexture, TexCoords).rgb,1.0);
	
	const float LOG2 = 1.442695;
	float z = LinearizeDepth(texture(screenDepth, TexCoords).r, viewPort.near, viewPort.far);

	float fogFactor = exp2( -fog.density * fog.density * z * z * LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	FragColor = mix(vec4(fog.color, 1.0), scenceColor, fogFactor );
} 