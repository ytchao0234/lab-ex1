#version 460 compatibility

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    float norm = vec3
    lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuse;

    FragColor = vec4(diffuse, 1.0);
}