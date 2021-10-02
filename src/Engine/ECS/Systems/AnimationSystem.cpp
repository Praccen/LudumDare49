#include "AnimationSystem.hpp"
#include "ECS/Components/GraphicsComponent.hpp"

#include <algorithm>

AnimationSystem::AnimationSystem(ECSManager* ECSManager) 
: System(ECSManager, ComponentTypeEnum::GRAPHICS){

}

void AnimationSystem::update(float dt) {
	//System should only have one entity
	for (auto& e : m_entities) {
		GraphicsComponent* graphComp = static_cast<GraphicsComponent*>(e->getComponent(ComponentTypeEnum::GRAPHICS));
		if (!graphComp->animate) {
			continue;
		}

		graphComp->updateTimer += dt;
        float advancements = std::floor(graphComp->updateTimer / std::max(graphComp->updateInterval, 0.00001f));

        float xAdvance = std::fmod((graphComp->advanceBy.x * advancements), std::max(graphComp->modAdvancement.x, 1.0f));
        float yAdvance = std::fmod((graphComp->advanceBy.y * advancements), std::max(graphComp->modAdvancement.y, 1.0f));
        graphComp->quad->setCurrentSprite(graphComp->startingTile.x + xAdvance, graphComp->startingTile.y + yAdvance);
	}
}