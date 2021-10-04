#include "ECS/Systems/PowerUpSystem.hpp"
#include "ECS/Components/GraphicsComponent.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/MovementComponent.hpp"
#include "ECS/Components/CollisionComponent.hpp"
#include "ECS/ECSManager.hpp"
#include "Rendering.hpp"

PowerUpSystem::PowerUpSystem(ECSManager* ECSManager) 
	: System(ECSManager, ComponentTypeEnum::POWERUP, ComponentTypeEnum::PLAYER),
	powerWeights() {
}

void PowerUpSystem::update(float dt) {
	bool shouldSpawn = (rand() % 100 + 1) > 5;
	if (shouldSpawn) {
		spawnPowerUp();
	}

	for (auto& e : m_entities) {

		PowerUpComponent* powerComp = static_cast<PowerUpComponent *>(e->getComponent(ComponentTypeEnum::POWERUP));

		for (int i = 0; i < powerComp->type.size(); i++) {
			switch(powerComp->type[i]) {
			case PowerUpType::Hen :
				//do hen stuff
				//save the players life!
				//add uppwards speed
				//add damagecomponent
				//start han saving timer
				//if hen saving timer is out remove damagecomponent
				break;
			default:
				break;
			}
		}
	}

}
void PowerUpSystem::spawnPowerUp(){
	//int totalWeight = 0;
	//for (auto &w : powerWeights) {
	//	totalWeight += w.second;
	//}

	//int selector = rand() % totalWeight + 1;


	Entity& powerup = m_manager->createEntity();
	PowerUpComponent* powerComp = new PowerUpComponent();
	//s�tt type beroende p� vad som randomats fram
	powerComp->type[0] = PowerUpType::Hen;
	m_manager->addComponent(powerup, powerComp);

	GraphicsComponent* graphComp = new GraphicsComponent();
	graphComp->quad->setTextureIndex(2);
	graphComp->quad->setNrOfSprites(1.0f, 1.0f);
	graphComp->quad->setCurrentSprite(0.0f, 0.0f);
	m_manager->addComponent(powerup, graphComp);

	glm::vec2 pos = Rendering::getInstance().getCamera()->getPosition();
	PositionComponent* posComp = new PositionComponent(pos.x, pos.y);
	m_manager->addComponent(powerup, posComp);

	m_manager->addComponent(powerup, new MovementComponent());
	m_manager->addComponent(powerup, new CollisionComponent());
}
//randoma vilken typ av powerup som ska spawna. Divktade efter hur bra de �r
//vissa kanske bara kan komma upp senare i spelet?

//titta vilka powerups som �r upplockade av player. l�gg det i playerkomponenten i kollisionssystem kanske?
//m�jligt att mna kan g�ra s� powerups �r antingen kontrollerade( tagna) eller att de �r fria (kan bli tagna)
//tagna/fria				finns i palyerkomponent
//-player kan bara ha en
//+man kan g�ra alla powerupgrejer h�r
//-fast det kan man inte

//se om de aktiveras
//f�r h�na titta om playerns position �r f�r l�g. ge playern d� speed upp och damagekomponent s� den f�rst�r tiles
//f�r dubbelhopp 