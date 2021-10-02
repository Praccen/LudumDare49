#version 440
precision highp float;

// in vec4 color;
in vec2 texCoords;
in flat int textureId;

// If uniforms change, also update SimpleShaderProgram to match
layout(location = 1) uniform sampler2D texture0;
layout(location = 2) uniform sampler2D texture1;
layout(location = 3) uniform sampler2D texture2;
layout(location = 4) uniform sampler2D texture3;
layout(location = 5) uniform sampler2D texture4;
layout(location = 6) uniform sampler2D texture5;
layout(location = 7) uniform sampler2D texture6;
layout(location = 8) uniform sampler2D texture7;
layout(location = 9) uniform sampler2D texture8;
layout(location = 10) uniform sampler2D texture9;

out vec4 FragColor;

mat4 thresholdMatrix = mat4(
    1.0, 9.0, 3.0, 11.0,
    13.0, 5.0, 15.0, 7.0,
    4.0, 12.0, 2.0, 10.0,
    16.0, 8.0, 14.0, 6.0
    );

void main()
{
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    if (textureId == 0) {
        FragColor = texture(texture0, texCoords);
    }
    else if (textureId == 1) {
        FragColor = texture(texture1, texCoords);
    }
    else if (textureId == 2) {
        FragColor = texture(texture2, texCoords);
    }
    else if (textureId == 3) {
        FragColor = texture(texture3, texCoords);
    }
    else if (textureId == 4) {
        FragColor = texture(texture4, texCoords);
    }
    else if (textureId == 5) {
        FragColor = texture(texture5, texCoords);
    }
    else if (textureId == 6) {
        FragColor = texture(texture6, texCoords);
    }
    else if (textureId == 7) {
        FragColor = texture(texture7, texCoords);
    }
    else if (textureId == 8) {
        FragColor = texture(texture8, texCoords);
    }
    else if (textureId == 9) {
        FragColor = texture(texture9, texCoords);
    }


    // Screen-door transparancy
    float threshold = thresholdMatrix[int(floor(mod(gl_FragCoord.x, 4)))][int(floor(mod(gl_FragCoord.y, 4)))] / 17;
    if (threshold >= FragColor.a) {
        discard;
    }
}
