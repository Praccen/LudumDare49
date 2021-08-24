#include "InputSystem.h"
#include "../Components/InputComponent.h"
#include "../Components/MovementComponent.h"
#include "../ECSManager.h"

InputSystem::InputSystem(ECSManager *ECSManager) 
	: System(ECSManager, ComponentTypeEnum::INPUT, ComponentTypeEnum::MOVEMENT){
}


void InputSystem::update(float dt){
	//Handle input


	for (auto& e : m_entities) {
		//Get component(s)
		InputComponent* input = static_cast<InputComponent*>(e->getComponent(ComponentTypeEnum::INPUT));
		MovementComponent* movement = static_cast<MovementComponent*>(e->getComponent(ComponentTypeEnum::MOVEMENT));

		//calculate input. Maybe every AI can use its own input component for this?
		input->calculateInput();

		//movementInput
		glm::vec3 direction(0.0f);
		if (input->m_keys[GLFW_KEY_W] == GLFW_PRESS) {
			direction.y += 1.0f;
		}
		if (input->m_keys[GLFW_KEY_S] == GLFW_PRESS) {
			direction.y += -1.0f;
		}
		if (input->m_keys[GLFW_KEY_A] == GLFW_PRESS) {
			direction.x += -1.0f;
		}
		if (input->m_keys[GLFW_KEY_D] == GLFW_PRESS) {
			direction.x += 1.0f;
		}
		if (input->m_keys[GLFW_KEY_SPACE] == GLFW_PRESS) {
			m_manager->removeComponent(*e, ComponentTypeEnum::COLLISION);
		}
		movement->m_accelerationDirection.x = direction.x;
		movement->m_accelerationDirection.y = direction.y;
	}
}