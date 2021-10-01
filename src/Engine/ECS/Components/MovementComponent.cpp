#include "MovementComponent.hpp"
#include "MapLoader/MapLoader.hpp"

MovementComponent::MovementComponent() :
	constantAcceleration(0.0f),
	accelerationDirection(0.0f),
	velocity(0.0f),
	wantedVelocity(0.0f),
	acceleration(5.0f),
	drag(10.0f),
	maxSpeed(7.0f),
	jumpRequested(false),
	jumpAllowed(true),
	jumpPower(8.0f) {
	m_componentType = ComponentTypeEnum::MOVEMENT;
}

MovementComponent::MovementComponent(float velX, float velY) {
	MovementComponent();
	velocity.x = velX;
	velocity.y = velY;
}
