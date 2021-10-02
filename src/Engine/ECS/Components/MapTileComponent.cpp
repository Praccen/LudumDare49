#include "MapTileComponent.hpp"

MapTileComponent::MapTileComponent() 
    : unstable(false) {
    m_componentType = ComponentTypeEnum::MAPTILE;
}