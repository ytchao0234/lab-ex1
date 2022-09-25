#version 460 compatibility

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;

void main()
{
    vec3 clothColor = vec3(0.6f, 0.3f, 0.1f);

    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(lightPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
    // vec3 specular = specularStrength * spec * lightColor;
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);

    FragColor = vec4((diffuse + ambient + specular) * clothColor , 1.0);
}