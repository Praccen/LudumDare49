#include "GraphicsComponent.hpp"
#include "Rendering.hpp"

#include "Objects/InstancedQuadManager.hpp"

GraphicsComponent::GraphicsComponent():
	startingTile(0.0f),
	advanceBy(0.0f),
	updateInterval(1.0f),
	modAdvancement(1.0f),
	updateTimer(0.0f) {
	quad = Rendering::getInstance().getNewQuad();

	m_componentType = ComponentTypeEnum::GRAPHICS;
}

GraphicsComponent::~GraphicsComponent() {
	Rendering::getInstance().getQuadManager()->returnQuad(quad);
}
