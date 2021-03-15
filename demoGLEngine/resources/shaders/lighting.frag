#version 330 core
out vec4 FragColor;

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

uniform sampler2D textureDiffuse;
uniform sampler2D textureSpecular;
uniform sampler2D textureMask;

in vec3 normalVec;
in vec3 fragPos; 
in vec2 texCoord;

void main() {

    // Diffuse textureDiffuse
    vec3 diffTexture = material.ambient * (vec3(texture(textureDiffuse, texCoord)) * material.diffuse * (( vec3(1.0) - vec3(texture(textureMask, texCoord)) )) + vec3(texture(textureMask, texCoord)) * vec3(texture(textureDiffuse, texCoord)));

    // Ambiant
    vec3 ambient = light.ambient * diffTexture;

    // Diffuse
    vec3 norm = normalize(normalVec);
    vec3 lightDir = normalize(light.position - fragPos); 

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse  *  diff * diffTexture;

    // Specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular * vec3(texture(textureSpecular, texCoord)));

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}