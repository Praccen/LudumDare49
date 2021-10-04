#pragma once
#include "System.hpp"
#include "../Components/PowerUpComponent.hpp"
#include <map>

class PowerUpSystem : public System {

public:

	PowerUpSystem(ECSManager* ECSManager);

	void update(float dt);

private:
	void spawnPowerUp();
	std::map<PowerUpType, int> powerWeights;
};

