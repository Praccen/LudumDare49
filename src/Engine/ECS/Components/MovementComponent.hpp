#pragma once

#include "Component.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <cmath>

class MovementComponent : public Component
{
public:
	glm::vec3 constantAcceleration;
	glm::vec3 accelerationDirection;
	glm::vec3 velocity;
	glm::vec3 wantedVelocity;
	float acceleration;
	float drag;
	float maxSpeed;
	bool jumpRequested;
	bool jumpAllowed;
	float jumpPower;

	MovementComponent();
	MovementComponent(float velX, float velY);
};