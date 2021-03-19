#version 400


uniform bool isTrans;

in vec3 v_Normals;
in vec3 v_LightDir;


out vec4 out_Color;

void main() {
    // Does nothing...
    if(isTrans) {
        vec3 n = normalize(v_Normals);
        vec3 l = normalize(v_LightDir);

        float ndotl = max(dot(n, l), 0.0);


        out_Color = vec4(vec3(1.0), ndotl);
    } else {
        out_Color = vec4(0.0);
    }
}