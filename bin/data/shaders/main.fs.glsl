#version 400

// Uniform
uniform sampler2D tex0;

// Layout
in vec2 v_TexCoords;
out vec4 out_Color;

void main() {
    out_Color = texture(tex0, v_TexCoords);
}