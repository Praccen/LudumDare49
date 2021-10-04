#include "PowerUpComponent.hpp"

PowerUpComponent::PowerUpComponent() :
	type(), activated(false), henTime(1.0f), henTimer(0.0f){
	m_componentType = ComponentTypeEnum::POWERUP;
}