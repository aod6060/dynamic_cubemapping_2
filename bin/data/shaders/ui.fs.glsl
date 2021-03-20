#version 400


uniform sampler2D tex0;
uniform vec4 color;

// Varying
in vec2 v_TexCoords;

out vec4 out_Color;

void main() {

    vec4 c = texture(tex0, v_TexCoords);

    c = c * color;

    out_Color = c;
}