#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D screenTexture;

void main() {
    vec3 visionColor = vec3(0.1, 0.95, 0.2);
   
    vec4 color = texture2D(screenTexture, TexCoords);
    float green = dot(color.rgb, visionColor);

    FragColor = vec4(0.0,green,0.0,1.0);
} 