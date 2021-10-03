#version 440

layout(location=0) in vec3 inPos;
layout(location=1) in vec2 inTexCoord;

out vec2 texCoords;

void main()
{
    texCoords = inTexCoords;
    gl_Position = vec4(pos, 1.0);
    gl_Position = vec4(inPos, 0.0, 1.0); 
}