#version 400

// Attributes
layout(location = 0) in vec3 vertices;

// Uniforms
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = proj * view * model * vec4(vertices, 1.0);
}