#include "MovementComponent.hpp"

MovementComponent::MovementComponent() :
	constantAcceleration(0.0f),
	accelerationDirection(0.0f),
	velocity(0.0f),
	wantedVelocity(0.0f),
	maxAcceleration(5.0f),
	drag(10.0f),
	maxSpeed(7.0f),
	jumpRequested(false),
	jumpAllowed(true),
	jumpPower(8.0f),
	dashRequested(false),
	dashCooldown(2.0f),
	dashTimer(0.0f),
	dashPower(15.0f){
	m_componentType = ComponentTypeEnum::MOVEMENT;
}

MovementComponent::MovementComponent(float velX, float velY) :
	MovementComponent() {
	velocity.x = velX;
	velocity.y = velY;
}
