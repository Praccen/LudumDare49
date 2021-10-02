#pragma once 
#include "System.hpp"
#include "ECS/Entity.hpp"
#include <array>

class MapSystem : public System {

public:
    MapSystem(ECSManager *ECSManager);
    void initialize() override;
    void update(float dt);

private: 
    struct tile {
        float x, y;
    };
    const unsigned int m_numTiles{30};
    std::array<Entity*, 30> tiles;
    
};