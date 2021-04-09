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

uniform sampler2D shadowDepthMap;
uniform sampler2D casticTex;
uniform sampler2D casticDepthMap;

// Layout
in vec2 v_TexCoords;
//in vec3 v_Normals;
in vec3 v_mpos;
in vec3 v_LightDir;
in vec3 v_ViewDir;
in mat3 v_TBN;
in vec3 v_norm;
in vec4 v_FragPosLightSpace;

out vec4 out_Color;

float ShadowCalc(vec4 p) {
    vec3 projCoords = p.xyz / p.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowDepthMap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float bias = 0.005;

    //float shadow = (currentDepth - bias> closestDepth) ? 1.0 : 0.0;

    float shadow = 0.0;
    vec2 texSize = 1.0 / textureSize(shadowDepthMap, 0);

    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowDepthMap, projCoords.xy + vec2(x, y) * texSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    if(projCoords.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;
}

float casticShadowCalc(vec4 p) {
    vec3 projCoords = p.xyz / p.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(casticDepthMap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float bias = 0.005;

    //float shadow = (currentDepth - bias> closestDepth) ? 1.0 : 0.0;

    float shadow = 0.0;
    vec2 texSize = 1.0 / textureSize(casticDepthMap, 0);

    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(casticDepthMap, projCoords.xy + vec2(x, y) * texSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    if(projCoords.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;
}

vec4 casticTexCalc(vec4 p) {
    vec3 projCoords = p.xyz / p.w;

    projCoords = projCoords * 0.5 + 0.5;

    vec4 trans = texture(casticTex, projCoords.xy);

    return trans;
}


void main() {

    vec3 color = texture(albedo, v_TexCoords).rgb;
    //vec3 n = normalize(v_Normals);
    // Compute Normals
    vec3 n = texture(normalMap, v_TexCoords).rgb;
    n = n * 2.0 - 1.0;
    n = normalize(v_TBN * n);

    //n = normalize(v_norm);

    vec3 l = normalize(v_LightDir);
    vec3 v = normalize(v_ViewDir);
    vec3 h = normalize(v + l);

    float ndotl = max(dot(n, l), 0.0);

    vec3 a = ambient;
    vec3 d = diffuse * ndotl;
    
    float ndoth = max(dot(n, h), 0.0);

    float pm = texture(specularMap, v_TexCoords).r;

    float p = pow(ndoth, pm * 256.0);

    vec3 s = specular * p;

    //vec3 finalColor = (a + d * (1.0 - s)) * color + s;
    float shadow = ShadowCalc(v_FragPosLightSpace);

    vec3 finalColor;

    //vec4 castic = casticTexCalc(v_FragPosLightSpace);
    //float casticShadow = casticShadowCalc(v_FragPosLightSpace);

    //float shadow2 = (shadow + casticShadow) * 0.5;
    //float shadow2 = (shadow + casticShadow) * 0.5;

    //float m = 0.5;

    /*
    if(trans.a > 0.0) {
        finalColor = (a + (shadow - (1.0 - trans.a)) * (d)) * color + s;
    } else {
        finalColor = (a + (1.0 - shadow) * (d)) * color + s;
    }
    */

    //finalColor = (a + (1.0 - mix(shadow, 1.0 - trans.a, 0.45)) * (d)) * color + s;
    //finalColor = (a + (1.0 - shadow) * (1.0 - casticShadow) * (d)) * color + s + ((1.0 - shadow) * ((castic.rgb + color) * 0.5) * castic.a) + ((1.0 - shadow) * (castic.rgb) * pow(castic.a, 4.0));
    finalColor = (a + (1.0 - shadow) * d) * color + s;
    
    //out_Color = vec4(n, 1.0)
    out_Color = vec4(finalColor, 1.0);
}