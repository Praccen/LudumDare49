#include "CollisionSystem.hpp"
#include "Rendering.hpp"
#include "Physics/SAT.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/MovementComponent.hpp"
#include "ECS/Components/CollisionComponent.hpp"
#include "ECS/Components/MapTileComponent.hpp"



CollisionSystem::CollisionSystem(ECSManager *ECSManager)
	: System(ECSManager, ComponentTypeEnum::COLLISION, ComponentTypeEnum::POSITION) {

}


void CollisionSystem::update(float /*dt*/) {
	
	// Update position of shapes
	for (auto& e : m_entities) {
		PositionComponent* p = static_cast<PositionComponent*>(e->getComponent(ComponentTypeEnum::POSITION));
		CollisionComponent* c = static_cast<CollisionComponent*>(e->getComponent(ComponentTypeEnum::COLLISION));

		c->shape.setTransformMatrix(p->calculateMatrix());
		c->currentCollisionEntities.clear(); // Clear current collisions
	}


	for (auto& e : m_entities) {
		PositionComponent* p = static_cast<PositionComponent*>(e->getComponent(ComponentTypeEnum::POSITION));
		CollisionComponent* c = static_cast<CollisionComponent*>(e->getComponent(ComponentTypeEnum::COLLISION));
		MovementComponent* m = static_cast<MovementComponent*>(e->getComponent(ComponentTypeEnum::MOVEMENT));

		// Don't check against others if this is constraint or doesn't have movement component
		if(c->isConstraint || !m) {
			continue;
		}

		// Collide with other entities
		for (auto& e2 : m_entities) {
			if (e->getID() == e2->getID()) {
				continue;
			}

			CollisionComponent* c2 = static_cast<CollisionComponent*>(e2->getComponent(ComponentTypeEnum::COLLISION));

			glm::vec2 tempIntersectionAxis(0.0f);
			float tempIntersectionDepth = 0.0f;

			if (SAT::getIntersection(c->shape, c2->shape, tempIntersectionAxis, tempIntersectionDepth)) {
				if (c2->effectMovement) {
					if (glm::length2(tempIntersectionAxis) > 0.0001f) {
						p->position += glm::vec3(tempIntersectionAxis, 0.0f)  * tempIntersectionDepth;
						glm::vec3 normalizedIntersectionAxis = {glm::normalize(tempIntersectionAxis), 0.0f};
						m->velocity -= normalizedIntersectionAxis * glm::dot(normalizedIntersectionAxis, m->velocity);

						// Allow jumping if standing on ground pointing upwards
						if (normalizedIntersectionAxis.y > 0.6f) {
							m->jumpAllowed = true;
						}

						// Update shape
						c->shape.setTransformMatrix(p->calculateMatrix());
					}
				}
				c->currentCollisionEntities.emplace_back(e2); // Save collision
			}
		}

	}
}