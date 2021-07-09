#define NOMINMAX
#include "Game.hpp"

#include "../Engine/MapLoader/MapLoader.hpp"


Game::Game(Rendering& rendering, GLFWwindow* window):
	m_rendering(rendering),
	/*m_player(m_rendering.getNewQuad()),*/
	m_botLoader("myBot"),
	m_botInterface(m_botLoader.newInterface()),
	m_ECSManager()
{
	MapLoader::mapInstance->getModelMatrix() = glm::translate(glm::mat4(1.0f),
		glm::vec3(0.5f * (float) MapLoader::mapInstance->getWidth(), 0.5f * (float)MapLoader::mapInstance->getHeight(), 0.1f));
	MapLoader::mapInstance->getModelMatrix() = glm::scale(MapLoader::mapInstance->getModelMatrix(),
		glm::vec3((float) MapLoader::mapInstance->getWidth(), (float)MapLoader::mapInstance->getHeight(), 1.0f)); // Scale map to make the tile size 1x1
	m_rendering.getCamera()->setZoom(1.0f/(0.5f * (float) std::max(MapLoader::mapInstance->getWidth(), MapLoader::mapInstance->getHeight()))); // Zoom out so that the whole map is visible
	m_rendering.getCamera()->setPosition(0.5f * (float)MapLoader::mapInstance->getWidth(), 0.5f * (float)MapLoader::mapInstance->getHeight());
	m_botInterface->print();


	//Testing to create an entity here. Maybe they should be created in a factory?
	//I think all collaborators here love factories...
	Entity* playerEntity = new Entity(1);
	//L�gg till komponenter till player
	playerEntity->addComponent(new PositionComponent(m_rendering.getNewQuad()->getModelMatrix()));
	playerEntity->addComponent(new MovementComponent());
	playerEntity->addComponent(new InputComponent(window));
	playerEntity->addComponent(new CollisionComponent());
	m_ECSManager.addEntity(playerEntity);
}	

Game::~Game() {
	delete m_botInterface;
}

void Game::processInput(GLFWwindow* window) {
	glm::vec3 movement(0.0f);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		movement.y += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		movement.y += -1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		movement.x += -1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		movement.x += 1.0f;
	}
	//m_player.setAccelerationDirection(movement);
}

void Game::update(float dt) {
	//m_player.update(dt);
	m_ECSManager.update(dt);
}
