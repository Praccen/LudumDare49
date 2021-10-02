#include "PlayerComponent.hpp"

PlayerComponent::PlayerComponent() 
	: alive(true), score(0) {
	m_componentType = ComponentTypeEnum::PLAYER;
}