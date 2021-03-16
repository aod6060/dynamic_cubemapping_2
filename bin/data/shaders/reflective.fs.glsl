#version 400


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

in vec3 v_Normal;
in vec3 v_LightDir;
in vec3 v_ViewDir;
in vec4 v_FragPosLightSpace;
in vec3 v_Irradiance;

uniform sampler2D shadowMap;

uniform samplerCube refTex;

out vec4 out_Color;

float ShadowCalc(vec4 p) {
    vec3 projCoords = p.xyz / p.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float bias = 0.005;

    //float shadow = (currentDepth - bias> closestDepth) ? 1.0 : 0.0;

    float shadow = 0.0;
    vec2 texSize = 1.0 / textureSize(shadowMap, 0);

    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    if(projCoords.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;
}

void main() {
    //vec3 color = texture(refTex, v_TexCoords).rgb;
    vec3 I = normalize(v_Irradiance);

    
    vec3 n = normalize(v_Normal);
    vec3 l = normalize(v_LightDir);
    vec3 v = normalize(v_ViewDir);

    /*
    vec3 R = reflect(I, n);
    vec3 R = refract(I, n, 1.2);
    */

    //vec3 R = reflect(I, n) * 0.2 + refract(I, n, 1.2) * 0.8;
    vec3 R1 = reflect(I, n);
    vec3 R2 = refract(I, n, 1.1);

    vec3 c1 = texture(refTex, R1).rgb;
    vec3 c2 = texture(refTex, R2).rgb;

    vec3 color = mix(c2, c1, 0.4);

    vec3 h = normalize(v + l);

    float ndotl = max(dot(n, l), 0.0);

    vec3 a = ambient;
    vec3 d = diffuse * ndotl;

    float ndoth = max(dot(n, h), 0.0);
    float p = pow(ndoth, 128.0);

    vec3 s = specular * p;

    float shadow = ShadowCalc(v_FragPosLightSpace);

    vec3 finalColor = (a + (1.0 - shadow) * (d)) * color + s * (1.0 - shadow);

    out_Color = vec4(finalColor, 1.0);
}
