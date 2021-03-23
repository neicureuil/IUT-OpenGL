#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool hasDeformation;
uniform vec2 deformationPos;
uniform vec3 deformationDir;
uniform vec3 deformationColor;
uniform float deformationSpeed;

out vec3 normalVec;  
out vec2 texCoord;
out float isDeformed;
out vec3 defColor;

void main() {
    
    vec3 gPos = aPos;
    
    vec3 pPos = vec3(aTexCoord.x, aTexCoord.y, 0.0);
    vec3 defPos = vec3(1-deformationPos.x, 1-deformationPos.y, 0.0);

    isDeformed = 0.0;
    defColor = deformationColor;

    if(hasDeformation) {
        float d = distance(defPos, pPos);
        if (d < 0.1) {
            isDeformed = 1.0;
		    //gPos.xyz = gPos + deformationSpeed * deformationDir;
	    }
    }
    
    gl_Position = projection * view * model * vec4(gPos, 1.0);
    texCoord = aTexCoord;
}
    
  
