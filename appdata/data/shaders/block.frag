
#version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

out vec4 outputC;

uniform sampler2D uTexture;
uniform float uShininess;
uniform vec4 uDiffuse;
uniform vec4 uSpecular;
uniform vec4 uAmbient;

in vec4 position;
in vec4 normal;
in vec2 uv;
in vec4 lightPos;

void main (void) {
  vec4 color = texture(uTexture, uv);
  vec3 light = normalize((lightPos * position.w - lightPos.w * position).xyz);
  vec3 fnormal = normalize(normal.xyz);
  float diffuse = max(dot(light, fnormal), 0.0);
  vec3 view = -normalize(position.xyz);
  vec3 halfway = normalize(light + view);
  float specular = pow(max(dot(fnormal, halfway), 0.0), uShininess);
  outputC = color * uDiffuse * diffuse
                + uSpecular * specular
                + color * uAmbient;
}
