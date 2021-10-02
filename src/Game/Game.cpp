#define NOMINMAX
#include "Game.hpp"

#include "Engine/Rendering.hpp"

#include <algorithm>

#include "Engine/ECS/Entity.hpp"
#include "Engine/MapLoader/MapLoader.hpp"

Game::Game(GLFWwindow* window):
	m_ECSManager(&ECSManager::getInstance())
{
	Rendering::getInstance().getQuadManager()->getTexture().loadFromFile("resources/Textures/instanced.png");
	Rendering::getInstance().getCamera()->setZoom(1.0f/(0.5f * 15.f, 15.f)); // Zoom out so that the whole map is visible
	Rendering::getInstance().getCamera()->setPosition(0.5f * 15.f, 0.5f * 15.f);
	
	m_ECSManager->createPlayerEntity(7.f, 4.f, window);
	}	

void Game::update(float dt) {
	m_ECSManager->update(dt);
}
