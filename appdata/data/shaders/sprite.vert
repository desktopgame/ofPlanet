#version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
// texture2D.vert

layout(location=0) in vec3 aVertex;
layout(location=3) in vec2 aUV;
out vec2 uv;

void main(void) {
  uv = aUV;
  gl_Position = vec4(aVertex, 1);
}