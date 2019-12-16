#version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

layout(location=0) in vec3 aVertex;
layout(location=2) in vec3 aNormal;
layout(location=3) in vec2 aUV;

uniform mat4 modelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec2 uv;

void main(void) {
  mat4 mvp = uProjectionMatrix * uViewMatrix * modelMatrix;
  vec4 position = mvp * vec4(aVertex, 1);
  gl_Position = position;
  uv = aUV;
}