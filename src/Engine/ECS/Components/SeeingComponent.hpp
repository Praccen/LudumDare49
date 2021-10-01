#pragma once

#include "Component.hpp"

#include <glm/glm.hpp>
#include <vector>

class Entity;

class SeeingComponent : public Component
{
public:
    struct VisualEntity {
        float distance{0.0f};
        float directionX{1.0f};
        float directionY{1.0f};
        int entityHit{-1};
    };

    float fov;
    unsigned int nrOfRays;
    float viewDistance;
	std::vector<VisualEntity> visualEntities; //List of entities that can be seen right now

	SeeingComponent();
};

