#pragma once 
#include "System.hpp"
#include "ECS/Entity.hpp"
#include "Rendering.hpp"
#include "ECS/Components/MapTileComponent.hpp"

#include <array>

class MapSystem : public System {

public:
    MapSystem(ECSManager *ECSManager);
    void initialize() override;
    void update(float dt);
    void increaseUnstable(); //increases chance of tile becoming unstable

private: 
    void createNewTile(float x, float y, MapTileComponent::TILE_TYPE t, float scaleX, float scaleY);
    void spawnObstacle();

    unsigned int m_numTiles{30};
    Rendering* m_render{nullptr};
    float m_lastTileY{1.0f};
    int m_destHeight{1};
    unsigned int m_drawnTiles{0};
    bool m_isPlatform{false};
    unsigned int m_numPlatformsDrawn{0};
    unsigned int m_platformHeight{0};

    int m_unstableChance{5}; //how many chances in 100 that a tile becomes unstable
};