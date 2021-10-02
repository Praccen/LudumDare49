#pragma once
#include "Component.hpp"

class MapTileComponent : public Component {
public:
    bool unstable;

    enum TILE_TYPE{
        GROUND,
        OBSTACLE
    };

    MapTileComponent() = delete;
    MapTileComponent(TILE_TYPE t);

    TILE_TYPE type;
};