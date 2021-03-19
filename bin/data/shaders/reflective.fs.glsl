#version 400

// Enviorment Mapping
#define ENV_REFLECT 0
#define ENV_REFRACT 1
#define ENV_GLASS 2

// Effects
#define FX_REGULAR 0
#define FX_BK 1
#define FX_SEPAI 2
#define FX_INVERT 3
#define FX_BLUR 4
#define FX_PIXELATED 5

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
in mat3 v_TBN;
in vec2 v_TexCoords;

uniform sampler2D shadowMap;
uniform sampler2D transMap;

uniform samplerCube refTex;
uniform sampler2D normalMap;

uniform bool isNormalMapped;
uniform int envType;
uniform int fxType;

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

vec4 transCalc(vec4 p) {
    vec3 projCoords = p.xyz / p.w;

    projCoords = projCoords * 0.5 + 0.5;

    vec4 trans = texture(transMap, projCoords.xy);

    return trans;
}

vec3 hurBlur(samplerCube s, vec3 tc, float blurSize) {
    vec3 sum = vec3(0.0);
    sum += texture(s, vec3(tc.x - 4.0*blurSize, tc.y, tc.z)).rgb * 0.05;
    sum += texture(s, vec3(tc.x - 3.0*blurSize, tc.y, tc.z)).rgb * 0.09;
    sum += texture(s, vec3(tc.x - 2.0*blurSize, tc.y, tc.z)).rgb * 0.12;
    sum += texture(s, vec3(tc.x - blurSize, tc.y, tc.z)).rgb * 0.15;
    sum += texture(s, vec3(tc.x, tc.y, tc.z)).rgb * 0.16;
    sum += texture(s, vec3(tc.x + blurSize, tc.y, tc.z)).rgb * 0.15;
    sum += texture(s, vec3(tc.x + 2.0*blurSize, tc.y, tc.z)).rgb * 0.12;
    sum += texture(s, vec3(tc.x + 3.0*blurSize, tc.y, tc.z)).rgb * 0.09;
    sum += texture(s, vec3(tc.x + 4.0*blurSize, tc.y, tc.z)).rgb * 0.05;
    return sum;
}

vec3 verBlur(samplerCube s, vec3 tc, float blurSize) {
    vec3 sum = vec3(0.0);
    sum += texture(s, vec3(tc.x , tc.y - 4.0*blurSize, tc.z)).rgb * 0.05;
    sum += texture(s, vec3(tc.x, tc.y - 3.0*blurSize, tc.z)).rgb * 0.09;
    sum += texture(s, vec3(tc.x, tc.y - 2.0*blurSize, tc.z)).rgb * 0.12;
    sum += texture(s, vec3(tc.x, tc.y - blurSize, tc.z)).rgb * 0.15;
    sum += texture(s, vec3(tc.x, tc.y, tc.z)).rgb * 0.16;
    sum += texture(s, vec3(tc.x, tc.y + blurSize, tc.z)).rgb * 0.15;
    sum += texture(s, vec3(tc.x, tc.y + 2.0*blurSize, tc.z)).rgb * 0.12;
    sum += texture(s, vec3(tc.x, tc.y + 3.0*blurSize, tc.z)).rgb * 0.09;
    sum += texture(s, vec3(tc.x, tc.y + 4.0*blurSize, tc.z)).rgb * 0.05;
    return sum;
}

vec3 depthBlur(samplerCube s, vec3 tc, float blurSize) {
    vec3 sum = vec3(0.0);
    sum += texture(s, vec3(tc.x , tc.y, tc.z - 4.0*blurSize)).rgb * 0.05;
    sum += texture(s, vec3(tc.x, tc.y, tc.z - 3.0*blurSize)).rgb * 0.09;
    sum += texture(s, vec3(tc.x, tc.y, tc.z - 2.0*blurSize)).rgb * 0.12;
    sum += texture(s, vec3(tc.x, tc.y, tc.z - blurSize)).rgb * 0.15;
    sum += texture(s, vec3(tc.x, tc.y, tc.z)).rgb * 0.16;
    sum += texture(s, vec3(tc.x, tc.y, tc.z + blurSize)).rgb * 0.15;
    sum += texture(s, vec3(tc.x, tc.y, tc.z + 2.0*blurSize)).rgb * 0.12;
    sum += texture(s, vec3(tc.x, tc.y, tc.z + 3.0*blurSize)).rgb * 0.09;
    sum += texture(s, vec3(tc.x, tc.y, tc.z + 4.0*blurSize)).rgb * 0.05;
    return sum;
}

vec3 blur3D(samplerCube s, vec3 tc, float blurSize) {
    return (hurBlur(s, tc, blurSize) + verBlur(s, tc, blurSize) + depthBlur(s, tc, blurSize)) / 3.0;
}

/*
    vec3 uv = R2;
    vec3 t = uv;
    float pixelSize = 1024.0;

    float dx = 8.0 * (1.0 / pixelSize);
    float dy = 8.0 * (1.0 / pixelSize);
    float dz = 8.0 * (1.0 / pixelSize);

    t.x = dx * floor(uv.x/dx);
    t.y = dy * floor(uv.y/dy);
    t.z = dz * floor(uv.z/dz);
*/

vec3 pixels(vec3 tc) {
    vec3 uv = tc;
    vec3 t;
    float pixelSize = 1024;

    float dx = 8.0 * (1.0 / pixelSize);
    float dy = 8.0 * (1.0 / pixelSize);
    float dz = 8.0 * (1.0 / pixelSize);

    t.x = dx * floor(uv.x / dx);
    t.y = dy * floor(uv.y / dy);
    t.z = dz * floor(uv.z / dz);
    return t;
}

void main() {
    //vec3 color = texture(refTex, v_TexCoords).rgb;
    vec3 I = normalize(v_Irradiance);

    vec3 n = vec3(0.0);

    if(isNormalMapped) {
        n = texture(normalMap, v_TexCoords).rgb;
        n = n * 2.0 - 1.0;
        n = normalize(v_TBN * n);
    } else {
        n = normalize(v_Normal);
    }
    
    vec3 l = normalize(v_LightDir);
    vec3 v = normalize(v_ViewDir);

    vec3 R, R1, R2;
    vec3 color;

    if(envType == ENV_REFLECT) {
        R = reflect(I, n);
    } else if(envType == ENV_REFRACT) {
        R = refract(I, n, 1.1);
    } else if(envType == ENV_GLASS) {
        R1 = reflect(I, n);
        R2 = refract(I, n, 1.1);
    }

    // Section for pixelate
    /*
        vec3 uv = R2;
        vec3 t = uv;
        float pixelSize = 1024.0;

        float dx = 8.0 * (1.0 / pixelSize);
        float dy = 8.0 * (1.0 / pixelSize);
        float dz = 8.0 * (1.0 / pixelSize);

        t.x = dx * floor(uv.x/dx);
        t.y = dy * floor(uv.y/dy);
        t.z = dz * floor(uv.z/dz);
    */
    if(fxType == FX_PIXELATED) {
        if(envType == ENV_GLASS) {
            R1 = pixels(R1);
            R2 = pixels(R2);
        } else {
            R = pixels(R);
        }
    }

    if(envType == ENV_GLASS) {
        vec3 c1, c2;

        if(fxType == FX_BLUR) {
            c1 = blur3D(refTex, R1, 16.0 / 1024.0);
            c2 = blur3D(refTex, R2, 16.0 / 1024.0);
        } else {
            c1 = texture(refTex, R1).rgb;
            c2 = texture(refTex, R2).rgb;
        }
        color = mix(c2, c1, 0.1);
    } else {
        if(fxType == FX_BLUR) {
            color = blur3D(refTex, R, 16.0 / 1024.0);
        } else {
            color = texture(refTex, R).rgb;
        }
    }


    // Black White
    if(fxType == FX_BK) {
        color = vec3((color.r + color.g + color.b) / 3.0);
    }
    // Sepia
    else if(fxType == FX_SEPAI) {
        float grey = dot(color, vec3(0.299, 0.587, 0.114));
        color = grey * vec3(1.2, 1.0, 0.8);
    }

    // Invert
    else if(fxType == FX_INVERT) {
        color = 1.0 - color;
    }

    //vec3 R = reflect(I, n);
    
    //vec3 R = refract(I, n, 1.1);
    

    //vec3 R = reflect(I, n) * 0.2 + refract(I, n, 1.2) * 0.8;
    //vec3 R1 = reflect(I, n);
    //vec3 R2 = refract(I, n, 1.1);

    //vec3 c1 = texture(refTex, R1).rgb;
    //vec3 c2 = texture(refTex, R2).rgb;

    //vec3 c1 = texture(refTex, R1).rgb;
    //vec3 c2 = blur3D(refTex, R2, 16.0 / 1024.0);
    /*
    vec3 uv = R2;
    vec3 t = uv;
    float pixelSize = 1024.0;

    float dx = 8.0 * (1.0 / pixelSize);
    float dy = 8.0 * (1.0 / pixelSize);
    float dz = 8.0 * (1.0 / pixelSize);

    t.x = dx * floor(uv.x/dx);
    t.y = dy * floor(uv.y/dy);
    t.z = dz * floor(uv.z/dz);
    */
    //vec3 c2 = texture(refTex, R2).rgb;

    //vec3 color = mix(c2, c1, 0.1);

    // Invert
    //color = 1.0 - color;
    // Black White
    //color.rgb = vec3((color.r + color.g + color.b) / 3.0);
    // Sepia Filter
    //float grey = dot(color, vec3(0.299, 0.587, 0.114));
    //color = grey * vec3(1.2, 1.0, 0.8);

    //vec3 color = texture(refTex, R).rgb;

    vec3 h = normalize(v + l);

    float ndotl = max(dot(n, l), 0.0);

    vec3 a = ambient;
    vec3 d = diffuse * ndotl;

    float ndoth = max(dot(n, h), 0.0);
    float p = pow(ndoth, 128.0);

    vec3 s = specular * p;

    float shadow = ShadowCalc(v_FragPosLightSpace);

    vec3 finalColor;

    vec4 trans = transCalc(v_FragPosLightSpace);

    if(trans.a > 0.0) {
        finalColor = (a + (1.0 - shadow) * (d)) * color + s + trans.rgb * pow(trans.a, 16.0) * color;
    } else {
        finalColor = (a + (1.0 - shadow) * (d)) * color + s;
    }
    
    out_Color = vec4(finalColor, 1.0);
}
