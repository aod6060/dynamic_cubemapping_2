#version 400


uniform sampler2D tex0;
uniform sampler2D tex1;

uniform float value;

in vec2 v_TexCoords;
out vec4 out_Color;

void main() {

    vec4 c1 = texture(tex0, v_TexCoords);
    vec4 c2 = texture(tex1, v_TexCoords);

    vec4 c = mix(c1, c2, value);

    out_Color = c;
}