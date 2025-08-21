#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projTrans;
uniform mat4 viewTrans;

void main()
{
    TexCoords = aPos;
    vec4 pos = projTrans * viewTrans * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}