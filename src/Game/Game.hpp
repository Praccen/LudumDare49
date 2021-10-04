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
	
	void introInit();
	void introUpdate(float dt);
	void finishIntro();
	void update(float dt);

	void reset();

	unsigned int getScore() { return m_score; };
	std::string getSubs() { return m_subs; };

private:
	void init();
	void setupEntities();

	GLFWwindow* m_window;
	ECSManager* m_ECSManager;

	bool introRunning;
	float introTimer;

	int playerEntityId;
	int cameraEntityId;
	int backgroundEntityId;
	int sunEntityId;
	int farmerEntityId;
	unsigned int m_score;
	std::string m_subs;
};

