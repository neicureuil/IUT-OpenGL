#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform vec3 lightPos;

out vec3 normalVec;
out vec3 fragPos;  
out vec2 texCoord;

out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

void main() {
    fragPos = vec3(model * vec4(aPos, 1.0));
    texCoord = aTexCoord;
    normalVec = mat3(transpose(inverse(model))) * aNormal;


    // Normal map
    mat3 normalMatrix =  transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    //T = normalize(T - dot(T, N) * N);
    //vec3 B = cross(N, T);
    vec3 B = normalize(normalMatrix * aBitangent);
    mat3 TBN = transpose(mat3(T, B, N));    

    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * fragPos;


    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
    
  
