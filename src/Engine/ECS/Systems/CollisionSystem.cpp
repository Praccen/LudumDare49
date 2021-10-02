#include "CollisionSystem.hpp"
#include "Rendering.hpp"
#include "Physics/SAT.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/MovementComponent.hpp"
#include "ECS/Components/CollisionComponent.hpp"
#include "ECS/Components/MapTileComponent.hpp"
#include "ECS/Components/PlayerComponent.hpp"



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

			//if playerEntity has reached the limits of the screen
			if (e->hasComponent(ComponentTypeEnum::PLAYER)) {
				//limit to the right is set to the right limit
				if (p->position.x > Rendering::getInstance().getCamera()->getPosition().x + 10.0f) {
					p->position.x = Rendering::getInstance().getCamera()->getPosition().x + 10.0f;
					continue;
				}
				//limit to the left or bottom kills player
				if (p->position.x < Rendering::getInstance().getCamera()->getPosition().x - 15.0f || 
					p->position.y < Rendering::getInstance().getCamera()->getPosition().y - 10.0f) {
					static_cast<PlayerComponent*>(e->getComponent(ComponentTypeEnum::PLAYER))->alive = false;
					continue;
				}
			}

			CollisionComponent* c2 = static_cast<CollisionComponent*>(e2->getComponent(ComponentTypeEnum::COLLISION));

			glm::vec2 tempIntersectionAxis(0.0f);
			float tempIntersectionDepth = 0.0f;

			if (SAT::getIntersection(c->shape, c2->shape, tempIntersectionAxis, tempIntersectionDepth)) {
				if (c2->effectMovement) {
					if (glm::length2(tempIntersectionAxis) > 0.0001f) {

						// Climbing ontop of things / being pushed down
						bool reverse = false;
						float verticalIntersectionDepth = SAT::getOverlap(glm::vec3(0.0f, 1.0f, 0.0f), c->shape.getTransformedVertices(), c2->shape.getTransformedVertices(), reverse);
						if (verticalIntersectionDepth <= c->allowedClimbing) {
							tempIntersectionAxis = {0.0f, 1.0f};
							if (reverse) {
								tempIntersectionAxis.y = -1.0f;
							}
							tempIntersectionDepth = verticalIntersectionDepth;
						} 

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