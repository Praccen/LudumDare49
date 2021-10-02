#pragma once
#include "System.hpp"

class HealthSystem : public System {
public:
    HealthSystem(ECSManager* ECSManager);
    void update(float dt);
};