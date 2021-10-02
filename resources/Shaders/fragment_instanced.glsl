#version 460 core
precision highp float;

// in vec4 color;
in vec2 texCoords;
in flat int textureId;

// If uniforms change, also update SimpleShaderProgram to match
layout(location = 1) uniform sampler2D texture0;
layout(location = 2) uniform sampler2D texture1;

out vec4 FragColor;

mat4 thresholdMatrix = mat4(
    1.0, 9.0, 3.0, 11.0,
    13.0, 5.0, 15.0, 7.0,
    4.0, 12.0, 2.0, 10.0,
    16.0, 8.0, 14.0, 6.0
    );

void main()
{

    FragColor = texture(texture0, texCoords);
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    // if (textureId == 0) {
    //     FragColor = texture(texture0, texCoords);
    // }
    // else if (textureId == 1) {
    //     FragColor = texture(texture1, texCoords);
    // }
	

    // Screen-door transparancy
    float threshold = thresholdMatrix[int(floor(mod(gl_FragCoord.x, 4)))][int(floor(mod(gl_FragCoord.y, 4)))] / 17;
    if (threshold >= FragColor.a) {
        discard;
    }
}
