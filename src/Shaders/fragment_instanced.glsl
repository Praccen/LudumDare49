#version 460 core
precision highp float;

// in vec4 color;
in vec2 texCoords;

// If uniforms change, also update SimpleShaderProgram to match
layout(location = 1) uniform sampler2D texture0;
layout(location = 2) uniform int useTexture;

out vec4 FragColor;

void main()
{
    if (useTexture == 1) {
        FragColor = texture(texture0, texCoords);
    }
    else {
       FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }

}