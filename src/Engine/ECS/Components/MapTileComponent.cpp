#include "MapTileComponent.hpp"

MapTileComponent::MapTileComponent(TILE_TYPE t) : unstable(false), type(t) {
    m_componentType = ComponentTypeEnum::MAPTILE;
}