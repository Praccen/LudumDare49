#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Shape {
public:
    Shape();
    virtual ~Shape();

    void addVertex(glm::vec2);
    void addNormal(glm::vec2);
    void setPosition(glm::vec2);

    const std::vector<glm::vec2>& getVertices() const;
    const std::vector<glm::vec2>& getNormals() const;

    void moveShape(glm::vec2);
private:
    std::vector<glm::vec2> m_vertices;
    std::vector<glm::vec2> m_normals;
    glm::vec2 m_centerPosition;
};
