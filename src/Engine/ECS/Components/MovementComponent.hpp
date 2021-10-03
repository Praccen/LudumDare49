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
	glm::vec2 maxAcceleration;
	float drag;
	float maxSpeed;
	bool jumpRequested;
	bool dashRequested;
	bool jumpAllowed;
	float dashTimer;
	float dashFrequency;
	float jumpPower;
	float dashPower;

	MovementComponent();
	MovementComponent(float velX, float velY);
};