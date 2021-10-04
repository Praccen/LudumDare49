#include "CollisionSystem.hpp"
#include "Rendering.hpp"
#include "Physics/SAT.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/MovementComponent.hpp"
#include "ECS/Components/CollisionComponent.hpp"
#include "ECS/Components/MapTileComponent.hpp"
#include "ECS/Components/PlayerComponent.hpp"
#include "ECS/Components/PowerUpComponent.hpp"



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

		//if playerEntity has reached the limits of the screen
		if (e->hasComponent(ComponentTypeEnum::PLAYER)) {
			//limit to the right is set to the right limit
			if (p->position.x > Rendering::getInstance().getCamera()->getPosition().x + 14.0f) {
				p->position.x = Rendering::getInstance().getCamera()->getPosition().x + 14.0f;
			}
			//limit top is set to limit top
			if (p->position.y > Rendering::getInstance().getCamera()->getPosition().y + 14.0f) {
				p->position.y = Rendering::getInstance().getCamera()->getPosition().y + 14.0f;
			}
			//limit to the left or bottom kills player...unless player has a chicken!
			if (p->position.x < Rendering::getInstance().getCamera()->getPosition().x - 15.0f){
				static_cast<PlayerComponent*>(e->getComponent(ComponentTypeEnum::PLAYER))->alive = false;
				continue;
			}
			if(p->position.y < Rendering::getInstance().getCamera()->getPosition().y - 10.0f){
				PowerUpComponent* powerComp = static_cast<PowerUpComponent*>(e->getComponent(ComponentTypeEnum::POWERUP));
				bool saved = false;
				for (int i = 0; i < powerComp->type.size(); i++) {
					if (powerComp->type[i] == PowerUpType::Hen) {
						saved = true;
						powerComp->activated[i] = true;
					}
				}
				if (!saved) {
					static_cast<PlayerComponent*>(e->getComponent(ComponentTypeEnum::PLAYER))->alive = false;
					continue;
				}
			}
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
				if (c2->effectMovement && !m->dashIsDamaging) {
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
						float dotProd = glm::dot(normalizedIntersectionAxis, m->velocity);
						if (dotProd < 0.0f) {
							m->velocity -= normalizedIntersectionAxis * dotProd;
						}

						// Allow jumping if standing on ground pointing upwards
						if (normalizedIntersectionAxis.y > 0.6f) {
							m->jumpAllowed = true;
						}

						// Update shape
						c->shape.setTransformMatrix(p->calculateMatrix());
					}
				}
				c->currentCollisionEntities.emplace_back(e2); // Save collision
				//add to constraint entity that it has been hit, it wont check it by itself
				if (c2->isConstraint) {
					c2->currentCollisionEntities.emplace_back(e);
				}
			}
		}

	}
}