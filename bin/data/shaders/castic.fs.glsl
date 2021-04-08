#version 400

uniform sampler2D normalMap;
uniform bool isNormalMapped;

uniform bool isBack;

uniform vec3 surfaceColor;
uniform float refValue; // 0.5 ~ 1.1

in vec2 v_TexCoords;
in vec3 v_Normals;
in vec3 v_LightDir;
in mat3 v_TBN;

out vec4 out_Color;

void main() {
    vec3 n;

    if(isNormalMapped) {
        n = texture(normalMap, v_TexCoords).xyz;
        n = n * 2.0 - 1.0;
        n = normalize(v_TBN * n);
    } else {
        n = normalize(v_Normals);
    }
    
    vec3 l = normalize(v_LightDir);

    if(isBack) {
        l = -l;
    }

    float ndotl = max(dot(n, l), 0.0);

    out_Color = vec4(surfaceColor, ndotl);
}