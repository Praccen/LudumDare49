#pragma once
#include "System.h"

class MovementSystem : public System
{
public:

	MovementSystem(ECSManager *ECSManager);

	void update(float dt);

};