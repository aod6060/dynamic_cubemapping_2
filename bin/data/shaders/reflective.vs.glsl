#version 400


layout(location = 0) in vec3 vertices;
layout(location = 1) in vec3 normals;

// Uniforms
layout(std140) uniform Matrices {
    mat4 proj;
    mat4 view;
    mat4 lightSpaceMatrix;
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

out vec3 v_Normal;
out vec3 v_LightDir;
out vec3 v_ViewDir;
out vec4 v_FragPosLightSpace;
out vec3 v_Irradiance;

void main() {
    gl_Position = proj * view * model * vec4(vertices, 1.0);

    vec3 mpos = vec3(model * vec4(vertices, 1.0));
    v_Normal = normalize(mat3(normalMatrix) * normals);
    v_LightDir = normalize(-direction);
    v_ViewDir = normalize(cameraPos - mpos);

    v_FragPosLightSpace = lightSpaceMatrix * vec4(mpos, 1.0);

    v_Irradiance = normalize(mpos - cameraPos);
}
