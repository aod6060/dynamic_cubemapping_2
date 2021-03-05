#version 400

// Attribute
layout(location = 0) in vec3 vertices;

// Uniforms
layout(std140) uniform Matrices {
    mat4 proj;
    mat4 view;
};

uniform mat4 model;

// Varying

void main() {
    gl_Position = proj * view * model * vec4(vertices, 1.0);
}