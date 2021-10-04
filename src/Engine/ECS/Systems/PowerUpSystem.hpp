#pragma once
#include "System.hpp"
#include "../Components/PowerUpComponent.hpp"
#include <map>

class PowerUpSystem : public System {

public:

	PowerUpSystem(ECSManager* ECSManager);

	void update(float dt);

private:
	bool powerUpExists;
	float spawnChance; // chance of powerup spawning
	std::map<PowerUpType, int> powerWeights;

	void spawnPowerUp();
};

