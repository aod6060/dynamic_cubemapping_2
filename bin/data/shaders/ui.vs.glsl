#version 400

// Attributes
layout(location=0) in vec3 vertices;
layout(location=1) in vec2 texCoords;

// Uniform
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

// Varying
out vec2 v_TexCoords;

void main() {
    gl_Position = proj * view * model * vec4(vertices, 1.0);

    v_TexCoords = texCoords;
}