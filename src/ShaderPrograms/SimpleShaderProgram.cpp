#include "SimpleShaderProgram.hpp"

#include <iostream>

#include <glad/glad.h>

SimpleShaderProgram::SimpleShaderProgram():
    ShaderProgram("Shaders/vertex.glsl", "Shaders/fragment.glsl") {

    // Change if uniforms change in shaders, the map's values are set to match layout(location = x) in shaders.
    m_uniformBindings["modelMatrix"] = 0;
    m_uniformBindings["viewMatrix"] = 1;
	m_uniformBindings["textureMatrix"] = 2;
	m_uniformBindings["texture0"] = 3;
    m_uniformBindings["useTexture"] = 4;

	use(); // Start using the shader

	// Set texture0 uniform to be texture unit 0
	glUniform1i(m_uniformBindings["texture0"], 0);
}

SimpleShaderProgram::~SimpleShaderProgram() {

}

void SimpleShaderProgram::setupVertexAttributePointers() {
    // Change if input layout changes in shaders
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

unsigned int SimpleShaderProgram::getUniformLocation(std::string uniformName) {
	if (m_uniformBindings.find(uniformName) == m_uniformBindings.end()) {
		std::cout << "No uniform with name " << uniformName << "\n";
	}
	else {
		return m_uniformBindings[uniformName];
	}

    return NULL;
}
