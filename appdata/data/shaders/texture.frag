#version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
// texture.frag
 
out vec4 outputC;
uniform sampler2D uTexture;

in vec2 uv;

void main (void) {
  outputC = texture(uTexture, uv);
}