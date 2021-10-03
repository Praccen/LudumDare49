#include "BlurEffect.hpp"
#include <glad/glad.h>

BlurEffect::BlurEffect() : ShaderProgram("resources/shaders/vertex_blur.glsl", "resources/shaders/fragment_blur.glsl") {
	glUniform1i(m_uniformBindings["screenTexture"], 0);
	glUniform1i(m_uniformBindings["horizontal"], 0);
}

void BlurEffect::setupVertexAttributePointers() {
   // Change if input layout changes in shaders
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}