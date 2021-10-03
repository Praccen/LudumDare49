#pragma once
#include "Component.hpp"

class MapTileComponent : public Component {
public:
    bool unstable;
    bool destroyed; //when tile is destroyed
    bool wasDestroyed; //when tile should be reset after destruction

    enum TILE_TYPE{
        GROUND,
        OBSTACLE
    };

    MapTileComponent() = delete;
    MapTileComponent(TILE_TYPE t);

    TILE_TYPE type;
};