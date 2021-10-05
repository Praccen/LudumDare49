#define NOMINMAX
#include "Game.hpp"

#include "Engine/Rendering.hpp"

#include <algorithm>

#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/Components/PlayerComponent.hpp"
#include "Engine/ECS/Components/PositionComponent.hpp"
#include "Engine/ECS/Components/InputComponent.hpp"
#include "Engine/ECS/Components/GraphicsComponent.hpp"

Game::Game(GLFWwindow* window):
	gameState(GameState::Menu), m_window(window), m_ECSManager(&ECSManager::getInstance()), introRunning(true), introTimer(0.0f), m_score(0), m_subs("") {

	Rendering::getInstance().getQuadManager()->getTexture(0).loadFromFile("resources/Textures/Horse.png");
	Rendering::getInstance().getQuadManager()->getTexture(1).loadFromFile("resources/Textures/bg-blurr.png");
	Rendering::getInstance().getQuadManager()->getTexture(2).loadFromFile("resources/Textures/platta2.png");
	Rendering::getInstance().getQuadManager()->getTexture(3).loadFromFile("resources/Textures/sun.png");
	Rendering::getInstance().getQuadManager()->getTexture(4).loadFromFile("resources/Textures/Stable.png");
	Rendering::getInstance().getQuadManager()->getTexture(5).loadFromFile("resources/Textures/Farmer.png");
	Rendering::getInstance().getQuadManager()->getTexture(6).loadFromFile("resources/Textures/chicken.png");
	
	init();
}

Game::~Game()
{
	m_ECSManager->reset();
	//Rendering::getInstance().~Rendering();
}

void Game::reset() {
	Rendering::getInstance().reset();
	m_ECSManager->reset();

	init();
}	

void Game::introInit() {
	// Rendering::getInstance()->
}

void Game::introUpdate(float dt) {
	introTimer += dt;

	Entity* cameraEntity = m_ECSManager->getEntity(cameraEntityId);
	PositionComponent* cameraPosComp = static_cast<PositionComponent*>(cameraEntity->getComponent(ComponentTypeEnum::POSITION));

	Entity* farmerEntity = m_ECSManager->getEntity(farmerEntityId);
	PositionComponent* farmerPosComp = static_cast<PositionComponent*>(farmerEntity->getComponent(ComponentTypeEnum::POSITION));

	Entity* playerEntity = m_ECSManager->getEntity(playerEntityId);
	PositionComponent* playerPosComp = static_cast<PositionComponent*>(playerEntity->getComponent(ComponentTypeEnum::POSITION));

	if (introTimer < 3.0f) { // Pan to farmer
		cameraPosComp->position.x = 8.0f - ((8.0f - farmerPosComp->position.x) / 3.0f) * introTimer;
		cameraPosComp->position.y = 3.0f - ((3.0f - farmerPosComp->position.y) / 3.0f) * introTimer;
		Rendering::getInstance().getCamera()->setZoom(1.0f/(15.f/(1.0f + introTimer * 2.0f)));
	}
	else if (introTimer < 7.0f) { // Text about farmer
		m_subs = "This is the very mentally unstable farmer, Greger Carl Gustaf.";
	} 
	else if (introTimer < 11.0f) {
		m_subs = "He is so unstable he hears voices in his head.";
	}
	else if (introTimer < 15.0f) {
		m_subs = "The voices are telling him to shoot his beloved horse...";
	}
	else if (introTimer < 16.0f) {
		m_subs = "";
		cameraPosComp->position.x = farmerPosComp->position.x + (playerPosComp->position.x - farmerPosComp->position.x) * (introTimer - 15.0f);
	}
	else if (introTimer < 20.0f) {
		m_subs = "YOU!";
	}
	else if (introTimer < 22.0f) {
		cameraPosComp->position.y = farmerPosComp->position.y + (3.0f - farmerPosComp->position.y) * (introTimer - 20)/2.0f;
		Rendering::getInstance().getCamera()->setZoom(1.0f/(15.f/(7.0f - (introTimer - 20.0f) * 3.0f)));
	}
	else if (introTimer < 24.0f) {
		m_subs = "Now run!";
	}
	else {
		m_subs = "";
		finishIntro();
	}

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		finishIntro();
	}
}

void Game::finishIntro() {
	Entity* playerEntity =  m_ECSManager->getEntity(playerEntityId);
	m_ECSManager->addComponent(*playerEntity, new InputComponent(m_window));

	Entity* cameraEntity = m_ECSManager->getEntity(cameraEntityId);
	MovementComponent* cameraMovComp = static_cast<MovementComponent*>(cameraEntity->getComponent(ComponentTypeEnum::MOVEMENT));
	cameraMovComp->constantAcceleration = glm::vec3(0.7f, 0.0f, 0.0f);

	Entity* backgroundEntity = m_ECSManager->getEntity(backgroundEntityId);
	GraphicsComponent* backgroundGraphComp = static_cast<GraphicsComponent*>(backgroundEntity->getComponent(ComponentTypeEnum::GRAPHICS));
	backgroundGraphComp->animate = true;

	PositionComponent* playerPosComp = static_cast<PositionComponent*>(playerEntity->getComponent(ComponentTypeEnum::POSITION));
	PositionComponent* cameraPosComp = static_cast<PositionComponent*>(cameraEntity->getComponent(ComponentTypeEnum::POSITION));

	Rendering::getInstance().getCamera()->setZoom(1.0f/15.f);
	cameraPosComp->position.x = playerPosComp->position.x;
	cameraPosComp->position.y = 3.0f;

	m_subs = "";

	introRunning = false;
}

void Game::update(float dt) {
	m_ECSManager->update(dt);

	Entity* playerEntity = m_ECSManager->getEntity(playerEntityId);
	Entity* cameraEntity = m_ECSManager->getEntity(cameraEntityId);
	Entity* backgroundEntity = m_ECSManager->getEntity(backgroundEntityId);
	Entity* sunEntity = m_ECSManager->getEntity(sunEntityId);

	MovementComponent* playerMovComp = static_cast<MovementComponent*>(playerEntity->getComponent(ComponentTypeEnum::MOVEMENT));
	PlayerComponent* playerComp = static_cast<PlayerComponent*>(m_ECSManager->getEntity(playerEntityId)->getComponent(ComponentTypeEnum::PLAYER));
	MovementComponent* cameraMovComp = static_cast<MovementComponent*>(cameraEntity->getComponent(ComponentTypeEnum::MOVEMENT));
	PositionComponent* backgroundPosComp = static_cast<PositionComponent*>(backgroundEntity->getComponent(ComponentTypeEnum::POSITION));
	PositionComponent* sunPosComp = static_cast<PositionComponent*>(sunEntity->getComponent(ComponentTypeEnum::POSITION));

	sunPosComp->position.x = Rendering::getInstance().getCamera()->getPosition().x - 2.5f;
	backgroundPosComp->position.x = Rendering::getInstance().getCamera()->getPosition().x;

	// ---- Intro ----
	if (introRunning) {
		if (introTimer == 0.0f) {
			introInit();
		}
		
		introUpdate(dt);
		return;
	}
	// ---------------

	// ---- Live ----
	if (cameraMovComp->velocity.x > 4.0f) {
		cameraMovComp->constantAcceleration.x = 0.1f;
	}

	// Animation
	GraphicsComponent* playerGraphComp = static_cast<GraphicsComponent*>(playerEntity->getComponent(ComponentTypeEnum::GRAPHICS));
	if (playerMovComp->velocity.x < 3.0f) {
		playerGraphComp->advanceBy = {1.0f, 0.0f};
		playerGraphComp->startingTile = {0.0f, 1.0f};
		playerGraphComp->modAdvancement = {8.0f, 1.0f};
		playerGraphComp->updateInterval = 0.2f;
		playerGraphComp->movementMultiplier = 0.5f;
	} else {
		playerGraphComp->advanceBy = {1.0f, 0.0f};
		playerGraphComp->startingTile = {0.0f, 0.0f};
		playerGraphComp->modAdvancement = {6.0f, 1.0f};
		playerGraphComp->updateInterval = 0.2f;
		playerGraphComp->movementMultiplier = 0.35f;
	}

	playerMovComp->wantedVelocity = cameraMovComp->velocity;
	playerMovComp->maxAcceleration.x = playerMovComp->wantedVelocity.x;

	// Update score
	m_score = playerComp->score;

	if (playerComp->alive == false) {
		gameState = GameState::GameOver;
	}
	// --------------
}

void Game::init() {
	Rendering::getInstance().getCamera()->setZoom(1.0f/15.f);
	Rendering::getInstance().getCamera()->setPosition(0.5f * 15.f, 0.5f * 15.f);

	introRunning = true;
	introTimer = 0.0f;
	m_score = 0;

	setupEntities();
}

void Game::setupEntities() {
	playerEntityId = m_ECSManager->createPlayerEntity(11.f, 4.f, m_window);
	cameraEntityId = m_ECSManager->createCameraEntity();
	backgroundEntityId = m_ECSManager->createBackgroundEntity();
	sunEntityId = m_ECSManager->createSunEntity();
	m_ECSManager->createStableEntity();
	farmerEntityId = m_ECSManager->createFarmerEntity();
}