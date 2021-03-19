#version 400

// Attributes
layout(location = 0) in vec3 vertices;
layout(location = 1) in vec3 normals;

// Uniforms
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

uniform vec3 lightDir;

out vec3 v_Normals;
out vec3 v_LightDir;

void main() {
    gl_Position = proj * view * model * vec4(vertices, 1.0);

    v_Normals = normalize(mat3(normalMatrix) * normals);

    v_LightDir = normalize(-lightDir);
}