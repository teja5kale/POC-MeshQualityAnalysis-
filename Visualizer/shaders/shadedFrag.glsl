#version 330 core
in vec3 fragNormal;
in vec3 fragPosition;
in vec3 fragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 color;

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPosition);

    // Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    // Specular shading
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // Combine results
    vec3 ambient = 0.1 * fragColor;              // Use fragColor for ambient lighting
    vec3 diffuse = diff * fragColor;             // Use fragColor for diffuse lighting
    vec3 specular = spec * vec3(1.0, 1.0, 1.0);  // Specular remains white

    // Final color
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);

}
