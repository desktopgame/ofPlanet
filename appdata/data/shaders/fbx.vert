#version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
// texture.vert

layout(location=0) in vec3 aVertex;
layout(location=2) in vec3 aNormal;
layout(location=3) in vec2 aUV;

uniform mat4 uMVPMatrix;
//uniform mat4 uNormalMatrix;
//uniform vec4 uLightPos;

//out vec4 position;
//out vec4 normal;
out vec2 uv;
//out vec4 lightPos;
//flat out int InstanceID; 

void main(void) {
  vec4 position = uMVPMatrix * vec4(aVertex, 1);
  //lightPos = uMVPMatrix * uLightPos;
  //normal = normalize(uNormalMatrix * vec4(aNormal, 0));
  
  uv = aUV;
  gl_Position = position;
}