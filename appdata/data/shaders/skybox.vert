#version 330 core
in vec3 aVertex;

out vec3 TexCoords;

uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

void main()
{
    TexCoords = aVertex;
    vec4 pos = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aVertex, 1);
    gl_Position = pos.xyww;
}  