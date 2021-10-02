#pragma once

#include "Engine/Objects/Quad.hpp"
#include "Engine/ECS/ECSManager.hpp"

enum class GameState {
	Menu,
	Playing,
	GameOver
};

class Game {
public:
	GameState gameState;

	Game(GLFWwindow* window);
	~Game() = default;
	
	void update(float dt);
private:
	ECSManager* m_ECSManager;

	int playerEntityId;
	int cameraEntityId;

};

