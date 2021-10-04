#include "ECS/Systems/PowerUpSystem.hpp"
#include "ECS/Components/GraphicsComponent.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/MovementComponent.hpp"
#include "ECS/Components/CollisionComponent.hpp"
#include "ECS/ECSManager.hpp"
#include "Rendering.hpp"

PowerUpSystem::PowerUpSystem(ECSManager* ECSManager) 
	: System(ECSManager, ComponentTypeEnum::POWERUP, ComponentTypeEnum::COLLISION),
	m_powerUpExists(false), m_spawnChance(0.0f), m_powerWeights() {
}

void PowerUpSystem::update(float dt) {
	if (!m_powerUpExists) {
		bool shouldSpawn = (rand() % 900000 + 1) < m_spawnChance;
		if (shouldSpawn) {
			spawnPowerUp();
			m_spawnChance = 0.0f;
			m_powerUpExists = true;
		}
		m_spawnChance += dt;
	}
	

	for (auto& e : m_entities) {
		PositionComponent* posComp = static_cast<PositionComponent*>(e->getComponent(ComponentTypeEnum::POSITION));
		float camX = Rendering::getInstance().getCamera()->getPosition().x;

		if (e->hasComponent(ComponentTypeEnum::PLAYER)) {
			PowerUpComponent* playerPower = static_cast<PowerUpComponent*>(e->getComponent(ComponentTypeEnum::POWERUP));
			CollisionComponent* collComp = static_cast<CollisionComponent*>(e->getComponent(ComponentTypeEnum::COLLISION));

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

				//destroy powerup entity and mark that another can spawn
				m_manager->removeEntity(e2->getID());
				m_powerUpExists = false;
			}

			for (int i = 0; i < playerPower->type.size() ; i++) {
				switch (playerPower->type[i]) {
				case PowerUpType::Hen:
					//do hen stuff
					if (playerPower->activated[i]) {

						if (e->hasComponent(ComponentTypeEnum::MOVEMENT) && playerPower->henTimer < 0.0001f) {
							//save the players life!
							//add uppwards speed
							MovementComponent* movComp = static_cast<MovementComponent*>(e->getComponent(ComponentTypeEnum::MOVEMENT));
							movComp->velocity.y += 30.0f;
							playerPower->henTimer = 0.0f;
							//add damagecomponent
							//start han saving timer
						}
						else if(playerPower->henTimer > playerPower->henTime){
							//if hen saving timer is out remove damagecomponent and hen and activated on same index from players types
							//reset hen powerup
							playerPower->type.erase(playerPower->type.begin() + i);
							playerPower->activated.erase(playerPower->activated.begin() + i);
						}
						playerPower->henTimer += dt;
					}

					
					break;
				default:
					break;
				}
			}
		}
		else if ((camX - posComp->position.x) > 14.0f) {
			m_manager->removeEntity(e->getID());
			m_powerUpExists = false;
		}
		
	}

}
void PowerUpSystem::spawnPowerUp(){
	Entity& powerup = m_manager->createEntity();
	PowerUpComponent* powerComp = new PowerUpComponent();
	//s�tt type beroende p� vad som randomats fram
	powerComp->type.push_back(PowerUpType::Hen);
	m_manager->addComponent(powerup, powerComp);

	GraphicsComponent* graphComp = new GraphicsComponent();
	graphComp->quad->setTextureIndex(6);
	graphComp->quad->setNrOfSprites(1.0f, 1.0f);
	graphComp->quad->setCurrentSprite(0.0f, 0.0f);
	m_manager->addComponent(powerup, graphComp);

	glm::vec2 pos = Rendering::getInstance().getCamera()->getPosition();
	PositionComponent* posComp = new PositionComponent(pos.x+14.0f, pos.y-2.0f + rand() % 6);
	m_manager->addComponent(powerup, posComp);

	CollisionComponent* collisionComp = new CollisionComponent();
	collisionComp->effectMovement = false;
	collisionComp->isConstraint = true;

	m_manager->addComponent(powerup, collisionComp);
}