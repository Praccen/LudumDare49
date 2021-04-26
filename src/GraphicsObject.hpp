#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Texture.hpp"

class ShaderProgram;

class GraphicsObject {
public:
    GraphicsObject(ShaderProgram& ShaderProgram);
    virtual ~GraphicsObject();

    void changeShaderProgram(ShaderProgram &shaderProgram);
    void setModelMatrix(glm::mat4 modelMatrix);
    void prepareDraw();
    virtual void draw() = 0;

protected:
    ShaderProgram& p_shaderProgram;

    Texture p_texture;

    virtual void setVertexData(std::size_t dataSize, const void* data);
    virtual void setIndexData(std::size_t dataSize, const void* data);
private:
    unsigned int m_VBO;
    unsigned int m_VAO;
    unsigned int m_EBO; // Optional

    glm::mat4 m_modelMatrix;
    unsigned int m_modelMatrixLoc;

    unsigned int m_useTextureLoc;

    void init();
};
