#pragma once

#include "Engine/Objects/Quad.hpp"
#include "Engine/ECS/ECSManager.hpp"

class Game {
public:
	Game(GLFWwindow* window);
	~Game() = default;
	
	void update(float dt);
private:
	ECSManager* m_ECSManager;

};

