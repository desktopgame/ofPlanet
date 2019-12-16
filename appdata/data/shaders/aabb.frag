#version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
// texture.frag
 
out vec4 outputC;

void main (void) {
  outputC = vec4(1, 0, 0, 1);
}