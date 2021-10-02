#include "MapTileComponent.hpp"

MapTileComponent::MapTileComponent(TILE_TYPE t) : type(t) {
    m_componentType = ComponentTypeEnum::MAPTILE;
}