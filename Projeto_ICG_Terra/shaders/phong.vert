#version 120

uniform vec3 uLightPositionEye;

varying vec3 vNormal;
varying vec3 vPosition;
varying vec3 vLightDirection;
varying vec3 vViewDirection;
varying vec2 vTexCoord;
varying vec4 vColor;

void main()
{
    vec4 posicao = gl_ModelViewMatrix * gl_Vertex;

    vPosition = posicao.xyz;
    vNormal = gl_NormalMatrix * gl_Normal;
    vLightDirection = uLightPositionEye - posicao.xyz;
    vViewDirection = -posicao.xyz;
    vTexCoord = gl_MultiTexCoord0.xy;
    vColor = gl_Color;

    gl_Position = gl_ProjectionMatrix * posicao;
}
