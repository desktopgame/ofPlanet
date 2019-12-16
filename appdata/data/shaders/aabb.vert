#version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
// texture.vert

layout(location=0) in vec3 aVertex;

uniform mat4 uMVPMatrix;

void main(void) {
  vec4 position = uMVPMatrix * vec4(aVertex, 1);
  gl_Position = position;
}