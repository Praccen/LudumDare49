#pragma once 
#include "System.hpp"
#include "ECS/Entity.hpp"
#include "Rendering.hpp"

#include <array>

class MapSystem : public System {

public:
    MapSystem(ECSManager *ECSManager);
    void initialize() override;
    void update(float dt);

private: 

    void createNewTile(float x, float y);

    unsigned int m_numTiles{30};
    Rendering* m_render;
    float lastTileY{1.0f};
    
};