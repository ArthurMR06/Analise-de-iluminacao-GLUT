#version 120

uniform sampler2D uTexture;
uniform int uUseTexture;

uniform vec3 uGlobalAmbient;
uniform vec3 uLightDiffuse;
uniform vec3 uLightSpecular;
uniform vec3 uMaterialSpecular;
uniform float uShininess;

varying vec3 vNormal;
varying vec3 vPosition;
varying vec3 vLightDirection;
varying vec3 vViewDirection;
varying vec2 vTexCoord;
varying vec4 vColor;

void main()
{
    vec4 corBase = vColor;

    if (uUseTexture == 1)
        corBase *= texture2D(uTexture, vTexCoord);

    vec3 N = normalize(vNormal);
    vec3 L = normalize(vLightDirection);
    vec3 V = normalize(vViewDirection);

    // Termo difuso: depende do angulo entre a normal e a luz.
    float difusa = max(dot(N, L), 0.0);

    // Termo especular: cria o brilho refletido na superficie.
    float especular = 0.0;
    if (difusa > 0.0)
    {
        vec3 reflexao = reflect(-L, N);
        especular = pow(max(dot(reflexao, V), 0.0), uShininess);
    }

    vec3 ambiente = uGlobalAmbient * corBase.rgb;
    vec3 difusaFinal = uLightDiffuse * corBase.rgb * difusa;
    vec3 especularFinal = uLightSpecular * uMaterialSpecular * especular;

    vec3 resultado = ambiente + difusaFinal + especularFinal;
    gl_FragColor = vec4(resultado, corBase.a);
}
