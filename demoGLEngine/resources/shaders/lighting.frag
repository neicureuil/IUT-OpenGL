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
uniform sampler2D textureNormal;

in vec3 normalVec;
in vec3 fragPos; 
in vec2 texCoord;


in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

void main() {

    // Normal Map
    vec3 normal = texture(textureNormal, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);  

    // Diffuse textureDiffuse
    vec3 diffTexture = material.ambient * (vec3(texture(textureDiffuse, texCoord)) * material.diffuse * (( vec3(1.0) - vec3(texture(textureMask, texCoord)) )) + vec3(texture(textureMask, texCoord)) * vec3(texture(textureDiffuse, texCoord)));

    // Ambiant
    vec3 ambient = light.ambient * diffTexture;

    // Diffuse
    vec3 norm = normalize(normalVec);
    vec3 lightDir = normalize(light.position - fragPos); 
    //vec3 lightDir = normalize(TangentLightPos - TangentFragPos);

    //float diff = max(dot(norm, lightDir), 0.0);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse  *  diff * diffTexture;

    // Specular
    //vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  

    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular * vec3(texture(textureSpecular, texCoord)));

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);


    /*
    vec3 normal = texture(textureNormal, texCoord).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 color = texture(textureDiffuse, texCoord).rgb * light.diffuse;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
    */
}