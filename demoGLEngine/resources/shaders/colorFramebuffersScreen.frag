#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D screenDepth;


struct ViewPort {
	float near;
	float far;
};

uniform ViewPort viewPort;


void main()
{
    vec3 scenceColor = texture(screenTexture, TexCoords).rgb;
	FragColor = vec4(vec3(scenceColor), 1.0);
} 