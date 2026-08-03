//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.00));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Material {
    sampler2D tex;
    float ambient;
    float diffuse;
    float specular;
    float shiness;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 color;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirlight;
uniform SpotLight spotlight;

uniform sampler2D texture_diffuse1;

uniform samplerCube depthMap;
uniform vec3 lightPos;
uniform float far_plane;
uniform bool shadows;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texColor);

vec3 sampleOffsetDirections[20] = vec3[](
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float ShadowCalculation(vec3 fragPos) {
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;

    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (0.1 + (viewDistance / far_plane)) / 25.0;

    for(int i = 0; i < samples; ++i) {
        float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

void main() {
    vec3 texColor = texture(texture_diffuse1, TexCoords).rgb;

    //ambient
    vec3 ambient = dirlight.ambient * material.ambient * texColor;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dirlight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirlight.diffuse * material.diffuse * diff * texColor;

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shiness);
    vec3 specular = dirlight.specular * material.specular * spec;

    vec3 result = ambient + diffuse + specular;

    result+=CalcSpotLight(spotlight, norm, FragPos, viewDir, texColor);

    FragColor = vec4(result, 1.0);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texColor){
    vec3 lightDir=normalize(light.position - fragPos);

    //diffuse
    float diff=max(dot(normal, lightDir), 0.0);

    //specular
    vec3 reflectDir=reflect(-lightDir, normal);
    float spec=pow(max(dot(viewDir, reflectDir), 0.0), material.shiness);

    //attenuation
    float distance=length(light.position - fragPos);
    float attenuation=1.0/ (light.constant + light.linear*distance + light.quadratic*distance*distance);

    float theta=dot(lightDir, normalize(-light.direction));
    float epsilon=light.cutOff - light.outerCutOff;
    float intensity = clamp((theta-light.outerCutOff) / epsilon, 0.0, 1.0);

    float shadow=shadows ? ShadowCalculation(fragPos): 0.0;
    float shadowFactor=1.0-shadow;

    vec3 ambient=light.ambient * material.ambient * texColor * light.color;
    vec3 diffuse = light.diffuse * material.diffuse * diff * texColor * light.color * shadowFactor;
    vec3 specular=light.specular * material.specular * spec * light.color * shadowFactor;

    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);

}