#include "ECS/Systems/PowerUpSystem.hpp"
#include "ECS/Components/GraphicsComponent.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/MovementComponent.hpp"
#include "ECS/Components/CollisionComponent.hpp"
#include "ECS/ECSManager.hpp"
#include "Rendering.hpp"

PowerUpSystem::PowerUpSystem(ECSManager* ECSManager) 
	: System(ECSManager, ComponentTypeEnum::POWERUP, ComponentTypeEnum::PLAYER, ComponentTypeEnum::COLLISION),
	m_powerUpExists(false), m_existingPowerUp(0), m_spawnChance(0.0f), m_powerWeights() {
}

void PowerUpSystem::update(float dt) {
	if (!m_powerUpExists) {
		bool shouldSpawn = (rand() % 100 + 1) > m_spawnChance;
		if (shouldSpawn) {
			spawnPowerUp();
			m_spawnChance = 0.0f;
		}
		m_spawnChance += dt;
		m_powerUpExists = true;
	}


	for (auto& e : m_entities) {

		PowerUpComponent* playerPower = static_cast<PowerUpComponent *>(e->getComponent(ComponentTypeEnum::POWERUP));
		CollisionComponent* collComp = static_cast<CollisionComponent*>(e->getComponent(ComponentTypeEnum::COLLISION));

		//gå igenom alla powerups player kolliderat med
			//för varje powerup checka vilken typ de är och adda till playerns types
		for (auto& e2 : collComp->currentCollisionEntities) {
			PowerUpComponent* collisionPower = static_cast<PowerUpComponent*>(e2->getComponent(ComponentTypeEnum::POWERUP));
			if (!collisionPower) {
				//not a powerup!
				continue;
			}
			//check if powerup power should be added to player powerupcomponent
			bool powerExists = false;
			for (int i = 0; i < playerPower->type.size(); i++) {
				if (playerPower->type[i] == collisionPower->type.front()) {
					powerExists = true;
					break;
				}
			}
			//if power does not exist, add it to the player
			if (!powerExists) {
				playerPower->type.push_back(collisionPower->type.front());
				playerPower->activated.push_back(false);
			}

			//destroy powerup entity
			m_manager->removeEntity(e2->getID());
		}

		for (int i = 0; i < playerPower->type.size(); i++) {
			switch(playerPower->type[i]) {
			case PowerUpType::Hen :
				//do hen stuff
				
				
				if (e->hasComponent(ComponentTypeEnum::MOVEMENT)) {
					//save the players life!
					//add uppwards speed
					//add damagecomponent
					//start han saving timer
				}
				
				
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
	//for (auto &w : m_powerWeights) {
	//	totalWeight += w.second;
	//}

	//int selector = rand() % totalWeight + 1;


	Entity& powerup = m_manager->createEntity();
	PowerUpComponent* powerComp = new PowerUpComponent();
	//sätt type beroende på vad som randomats fram
	powerComp->type.push_back(PowerUpType::Hen);
	m_manager->addComponent(powerup, powerComp);

	GraphicsComponent* graphComp = new GraphicsComponent();
	graphComp->quad->setTextureIndex(2);
	graphComp->quad->setNrOfSprites(1.0f, 1.0f);
	graphComp->quad->setCurrentSprite(0.0f, 0.0f);
	m_manager->addComponent(powerup, graphComp);

	glm::vec2 pos = Rendering::getInstance().getCamera()->getPosition();
	PositionComponent* posComp = new PositionComponent(pos.x, pos.y+8.0f);
	m_manager->addComponent(powerup, posComp);

	m_manager->addComponent(powerup, new MovementComponent());

	CollisionComponent* collisionComp = new CollisionComponent();
	collisionComp->isConstraint = true;
	m_manager->addComponent(powerup, collisionComp);
}
//randoma vilken typ av powerup som ska spawna. Divktade efter hur bra de är
//vissa kanske bara kan komma upp senare i spelet?

//titta vilka powerups som är upplockade av player. lägg det i playerkomponenten i kollisionssystem kanske?
//möjligt att mna kan göra så powerups är antingen kontrollerade( tagna) eller att de är fria (kan bli tagna)
//tagna/fria				finns i palyerkomponent
//-player kan bara ha en
//+man kan göra alla powerupgrejer här
//-fast det kan man inte

//se om de aktiveras
//för höna titta om playerns position är för låg. ge playern då speed upp och damagekomponent så den förstör tiles
//för dubbelhopp 