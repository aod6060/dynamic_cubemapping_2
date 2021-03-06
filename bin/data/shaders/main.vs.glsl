#version 400

// Attribute
layout(location = 0) in vec3 vertices;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normals;
layout(location = 3) in vec3 tangents;
layout(location = 4) in vec3 biTangents;

// Uniforms
layout(std140) uniform Matrices {
    mat4 proj;
    mat4 view;
};

layout(std140) uniform Light {
	vec3 direction;
    float nu1;
	vec3 ambient;
    float nu2;
	vec3 diffuse;
    float nu3;
	vec3 specular;
    float nu4;
};

uniform mat4 model;
uniform mat4 normalMatrix;
uniform vec3 cameraPos;

// Varying
out vec2 v_TexCoords;
//out vec3 v_Normals;
out vec3 v_mpos;
out vec3 v_LightDir;
out vec3 v_ViewDir;
out mat3 v_TBN;

void main() {
    gl_Position = proj * view * model * vec4(vertices, 1.0);
    v_TexCoords = texCoords;
    //v_Normals = normalize(mat3(normalMatrix) * normals);

    v_mpos = mat3(model) * vertices;
    v_LightDir = normalize(-direction);
    v_ViewDir = normalize(cameraPos - v_mpos);

    vec3 T = normalize(mat3(normalMatrix) * tangents);
    vec3 B = normalize(mat3(normalMatrix) * biTangents);
    vec3 N = normalize(mat3(normalMatrix) * normals);
    v_TBN = mat3(T, B, N);
}