#pragma once
#include "Component.hpp"

class MapTileComponent : public Component {
public:
    enum TILE_TYPE{
        GROUND,
        OBSTACLE
    };

    MapTileComponent() = delete;
    MapTileComponent(TILE_TYPE t);

    TILE_TYPE type;
};