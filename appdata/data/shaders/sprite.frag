#version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
// screen.frag
 
out vec4 outputC;
uniform sampler2D uTexture;
in vec2 uv;

void main (void) {
  vec4 color = texture(uTexture, uv);
  outputC = color;
}