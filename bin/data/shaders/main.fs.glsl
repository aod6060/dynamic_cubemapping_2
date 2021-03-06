#version 400

// Uniform
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

layout(std140) uniform Material {
    float power;
    vec3 nu;
};

uniform sampler2D albedo;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

// Layout
in vec2 v_TexCoords;
//in vec3 v_Normals;
in vec3 v_mpos;
in vec3 v_LightDir;
in vec3 v_ViewDir;
in mat3 v_TBN;

out vec4 out_Color;

void main() {

    vec3 color = texture(albedo, v_TexCoords).rgb;
    //vec3 n = normalize(v_Normals);
    // Compute Normals
    vec3 n = texture(normalMap, v_TexCoords).rgb;
    n = n * 2.0 - 1.0;
    n = normalize(v_TBN * n);

    vec3 l = normalize(v_LightDir);
    vec3 v = normalize(v_ViewDir);
    vec3 h = normalize(l + v);

    float ndotl = max(dot(n, l), 0.0);

    vec3 a = ambient;
    vec3 d = diffuse * ndotl;
    
    float ndoth = max(dot(n, h), 0.0);

    float pm = texture(specularMap, v_TexCoords).r;

    float p = pow(ndoth, pm * 256.0);

    vec3 s = specular * p;

    vec3 finalColor = (a + d * (1.0 - s)) * color + s;

    //out_Color = vec4(n, 1.0)
    out_Color = vec4(finalColor, 1.0);
}