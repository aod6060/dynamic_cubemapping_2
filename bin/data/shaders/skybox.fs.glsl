#version 400

uniform samplerCube skybox;

in vec3 v_TexCoords;
out vec4 out_Color;

void main() {
    out_Color = texture(skybox, v_TexCoords);
}

