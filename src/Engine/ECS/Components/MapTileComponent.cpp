#include "MapTileComponent.hpp"

MapTileComponent::MapTileComponent(TILE_TYPE t) : 
    unstable(false), destroyed(false), wasDestroyed(false), type(t) {
    m_componentType = ComponentTypeEnum::MAPTILE;
}