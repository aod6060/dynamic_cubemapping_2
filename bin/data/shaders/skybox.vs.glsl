#version 400

layout(location = 0) in vec3 vertices;

layout(std140) uniform Matrices {
    mat4 proj;
    mat4 view;
};

out vec3 v_TexCoords;

void main() {
    mat4 v = view;
    v[3][0] = 0;
    v[3][1] = 0;
    v[3][2] = 0;

    vec4 pos = proj * v * vec4(vertices, 1.0);
    gl_Position = pos.xyww;
    v_TexCoords = vertices;
}