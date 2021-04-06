/*
    Technically its called a caustic but I intentually 
    miss spelled it because it sould more okie to call
    it a castic instead... Okie Dad Jokes~
*/

#version 400

layout(location=0) in vec3 vertices;
layout(location=1) in vec2 texCoords;
layout(location=2) in vec3 normals;
layout(location=3) in vec3 tangents;
layout(location=4) in vec3 biTangents;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

uniform vec3 lightDir;

out vec2 v_TexCoords;
out vec3 v_Normals;
out vec3 v_LightDir;
out mat3 v_TBN;

void main() {
    gl_Position = proj * view * model * vec4(vertices, 1.0);

    v_TexCoords = texCoords;

    v_Normals = normalize(mat3(normalMatrix) * normals);

    v_LightDir = normalize(-lightDir);

    vec3 T = normalize(mat3(normalMatrix) * tangents);
    vec3 B = normalize(mat3(normalMatrix) * biTangents);

    v_TBN = mat3(T, B, v_Normals);
}