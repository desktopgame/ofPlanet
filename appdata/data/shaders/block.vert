 #version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
/*
0: position
1: color
2: normal
3: texcoord
*/
layout(location=0) in vec3 aVertex;
layout(location=2) in vec3 aNormal;
layout(location=3) in vec2 aUV;
layout(location=4) in vec3 aPosition;

uniform mat4 uMVPMatrix;
uniform mat4 uNormalMatrix;
//uniform vec4 uLightPos;

out vec4 position;
out vec4 normal;
out vec2 uv;
out vec4 lightPos;
//flat out int InstanceID; 

void main(void) {
  position = uMVPMatrix * vec4(aVertex+aPosition, 1);
  lightPos = uMVPMatrix * vec4(64,32,64, 1);
  normal = normalize(uNormalMatrix * vec4(aNormal, 0));
  uv = aUV;
  gl_Position = position;
}