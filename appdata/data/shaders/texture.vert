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

out vec2 uv;
//flat out int InstanceID; 

void main(void) {
  position = uMVPMatrix * vec4(aVertex+aPosition, 1);
  lightPos = uMVPMatrix * uLightPos;
  normal = normalize(uNormalMatrix * vec4(aNormal, 0));
  uv = aUV;
  gl_Position = position;
}