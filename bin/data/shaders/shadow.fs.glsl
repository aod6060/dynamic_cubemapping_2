#version 400


uniform bool isTrans;
uniform sampler2D normalMap;
uniform bool isNormalMapped;

in vec3 v_Normals;
in vec3 v_LightDir;
in mat3 v_TBN;
in vec2 v_TexCoords;

out vec4 out_Color;

void main() {
    // Does nothing...
    if(isTrans) {
        vec3 n;

        if(isNormalMapped) {
            n = texture(normalMap, v_TexCoords).xyz;
            n = n * 2.0 - 1.0;
            n = normalize(v_TBN * n);
        } else {
            n = normalize(v_Normals);
        }

        vec3 n2 = 1.0 - n;

        vec3 l = normalize(v_LightDir);

        float ndotl = max(dot(n, l), 0.0);
        float ndotl2 = max(dot(n, l), 0.0);

        out_Color = vec4(vec3(1.0), mix(ndotl, ndotl2, 0.5));
    } else {
        out_Color = vec4(0.0);
    }
}