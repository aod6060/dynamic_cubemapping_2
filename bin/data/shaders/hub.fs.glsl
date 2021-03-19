#version 400

uniform sampler2D tex0;

in vec2 v_TexCoords;

out vec4 out_Color;

void main() {
   //out_Color = texture(tex0, v_TexCoords);

   vec4 color = texture(tex0, v_TexCoords);

   if(color.a < 0.5) {
       discard;
   }

   out_Color = vec4(color.rgb * color.a, color.a);
}