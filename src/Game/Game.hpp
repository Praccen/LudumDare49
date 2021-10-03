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
	~Game();
	
	void update(float dt);

	void reset(GLFWwindow* window);

	unsigned int getScore() { return m_score; };

private:
	void setupEntities(GLFWwindow *window);

	ECSManager* m_ECSManager;

	int playerEntityId;
	int cameraEntityId;
	int backgroundEntityId;
	int sunEntityId;
	unsigned int m_score;

};

