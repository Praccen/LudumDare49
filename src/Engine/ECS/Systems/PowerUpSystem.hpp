#pragma once
#include "System.hpp"
#include "../Components/PowerUpComponent.hpp"
#include <map>

class PowerUpSystem : public System {

public:

	PowerUpSystem(ECSManager* ECSManager);

	void update(float dt);

private:
	bool m_powerUpExists;
	Entity &m_existingPowerUp;
	float m_spawnChance; // chance of powerup spawning
	std::map<PowerUpType, int> m_powerWeights;
	void spawnPowerUp();
};

