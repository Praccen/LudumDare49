#define NOMINMAX
#include "Game.hpp"

#include "Engine/Rendering.hpp"

#include <algorithm>
#include <functional>

#include "Engine/ECS/Entity.hpp"
#include "Engine/MapLoader/MapLoader.hpp"

Game::Game(GLFWwindow* window):
	m_botLoader(window),
	m_ECSManager(&ECSManager::getInstance()) {
	Rendering::getInstance().getMapLoader()->getModelMatrix() = glm::translate(glm::mat4(1.0f),
		glm::vec3(0.5f * (float) Rendering::getInstance().getMapLoader()->getWidth(), 
		0.5f * (float)Rendering::getInstance().getMapLoader()->getHeight(), 
		0.1f));
	Rendering::getInstance().getMapLoader()->getModelMatrix() = glm::scale(Rendering::getInstance().getMapLoader()->getModelMatrix(),
		glm::vec3((float) Rendering::getInstance().getMapLoader()->getWidth(), 
		(float)Rendering::getInstance().getMapLoader()->getHeight(), 
		1.0f)); // Scale map to make the tile size 1x1
	Rendering::getInstance().getCamera()->setZoom(1.0f/(0.5f * (float) std::max(Rendering::getInstance().getMapLoader()->getWidth(), 
		Rendering::getInstance().getMapLoader()->getHeight()))); // Zoom out so that the whole map is visible
	Rendering::getInstance().getCamera()->setPosition(0.5f * (float)Rendering::getInstance().getMapLoader()->getWidth(), 
		0.5f * (float)Rendering::getInstance().getMapLoader()->getHeight());
	
//	m_ECSManager->createPlayerEntity(7.f, 4.f, window);

	for(unsigned int i = 0; i < m_botLoader.m_bots.size(); i++) {
		BotInterface* b = m_botLoader.m_bots[i]->bot;
		std::function<void(BotInterface*, float*)> func = [](BotInterface* b, float* dRef) {
				b->update(dRef);
		};
		std::thread t(std::move(func), b, &m_dtRef);
		botThreads.push_back(std::move(t));
	}
}	

Game::~Game() {
	for (auto &&t : botThreads)	{
		t.join();
	}
}

void Game::update(float dt) {
	m_ECSManager->update(dt);
	m_dtRef = dt;
}
