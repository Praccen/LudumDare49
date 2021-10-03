#include "MovementSystem.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/MovementComponent.hpp"
#include "ECS/Components/DamageComponent.hpp"
#include "ECS/ECSManager.hpp"

MovementSystem::MovementSystem(ECSManager *ECSManager) 
	: System(ECSManager, ComponentTypeEnum::POSITION, ComponentTypeEnum::MOVEMENT){

}

void MovementSystem::update(float dt)
{
	for (auto& e : m_entities) {
		PositionComponent *p = static_cast<PositionComponent *>(e->getComponent(ComponentTypeEnum::POSITION));
		MovementComponent *m = static_cast<MovementComponent*>(e->getComponent(ComponentTypeEnum::MOVEMENT));

		
		m->dashTimer += dt;

		glm::vec3 oldVelocity = m->velocity;
		m->velocity += m->accelerationDirection * (glm::vec3(m->maxAcceleration, 0.0f) * dt);

		// Gravity
		m->velocity += m->constantAcceleration * dt;

		// Try to maintain the wanted velocity
		for (unsigned int i = 0; i < 3; i++) {
			if (m->wantedVelocity[i] != 0.0f) {
				// There is a wanted velocity in this axis, accelerate towards it
				m->velocity[i] += (m->wantedVelocity[i] - m->velocity[i]) * 2.0f * dt;
			}
		}

		// Jump
		if (m->jumpRequested && m->jumpAllowed) {
			m->velocity.y = m->jumpPower;
			m->jumpAllowed = false;
		}

		// Dash
		if (m->dashRequested && m->dashTimer >= m->dashFrequency) {
			m->velocity.x += m->dashPower;
			m->velocity.y = 3.0f;
			m->dashTimer = 0.0f;
			m->dashIsDamaging = true;
			m_manager->addComponent(*e, new DamageComponent());
		}
		if (m->dashTimer >= m->dashDamageTime && m->dashIsDamaging) {
			m->dashIsDamaging = false;
			m_manager->removeComponent(*e, ComponentTypeEnum::DAMAGE);
		}

		p->position += (oldVelocity + m->velocity) * 0.5f * dt; // This works for any update rate


		m->accelerationDirection = { 0.0f, 0.0f, 0.0f };
		m->jumpRequested = false;
		m->dashRequested = false;
	}
}
