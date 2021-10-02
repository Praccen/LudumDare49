#pragma once 
#include "System.hpp"

class PlayerSystem : public System {
public: 
    PlayerSystem(ECSManager* ECSManager);
    void update(float dt);
};