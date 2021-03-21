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

out vec3 normalVec;  
out vec2 texCoord;
out float isDeformed;

void main() {
    
    vec3 gPos = aPos;
    
    vec3 pPos = vec3(aTexCoord.x, aTexCoord.y, 0.0);
    vec3 defPos = vec3(1-deformationPos.x, 1-deformationPos.y, 0.0);

    isDeformed = 0.0;

    if(hasDeformation) {
        float d = distance(vec4(defPos, 1.0), vec4(pPos, 1.0));
        if (d < 0.1) {
            isDeformed = 1.0;
		    gPos.xyz = gPos.xyz + 5.0 * deformationDir.xyz;
	    }
    }
    
    gl_Position = projection * view * model * vec4(gPos, 1.0);
    texCoord = aTexCoord;
}
    
  
