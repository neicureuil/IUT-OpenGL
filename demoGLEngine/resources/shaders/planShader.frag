#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform sampler2D ground;

in vec3 normalVec;
in vec3 fragPos; 
in vec2 texCoord;

void main() {
    // Ambiant
    vec3 ambient = light.ambient * objectColor * lightColor ;

    // Diffuse
    vec3 norm = normalize(normalVec);
    vec3 lightDir = normalize(light.position - fragPos); 

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse  * ( diff );

    // Specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec);

    vec3 result = (ambient + diffuse + specular);
    FragColor = texture(ground, texCoord) * vec4(result, 1.0);
}