#include "PlayerSystem.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/PlayerComponent.hpp"

PlayerSystem::PlayerSystem(ECSManager* ECSManager) : System(ECSManager, ComponentTypeEnum::PLAYER, ComponentTypeEnum::POSITION) {}

void PlayerSystem::update(float /*dt*/) {
    for (auto &&e : m_entities) {
		PositionComponent *p = static_cast<PositionComponent *>(e->getComponent(ComponentTypeEnum::POSITION));
		PlayerComponent *pl = static_cast<PlayerComponent *>(e->getComponent(ComponentTypeEnum::PLAYER));
        pl->score = 10 * p->position.x ;
    }
}