#include "MapSystem.hpp"
#include "Objects/InstancedQuadManager.hpp"
#include "ECS/ECSManager.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/CollisionComponent.hpp"
#include "ECS/Components/GraphicsComponent.hpp"
#include "ECS/Components/MapTileComponent.hpp"

MapSystem::MapSystem(ECSManager *ECSManager) : System(ECSManager, ComponentTypeEnum::MAPTILE) {}

void MapSystem::initialize() {
    render =  &Rendering::getInstance();
    for(unsigned int i = 0; i < m_numTiles; ++i) {
        createNewTile(static_cast<float>(i), 1.0f);
    }
}

void MapSystem::update(float dt) {

    for(auto& e : m_entities) {
		PositionComponent *p = static_cast<PositionComponent *>(e->getComponent(ComponentTypeEnum::POSITION));
        float camX = render->getCamera()->getPosition().x;
        if((camX - p->position.x) > 10.f) {
            p->position.x = static_cast<float>(m_numTiles);
            m_numTiles++;
        }
    }
}

void MapSystem::createNewTile(float x, float y) {
        Entity& tileEntity = m_manager->createEntity();
        m_manager->addComponent(tileEntity, new PositionComponent(x, y));
        m_manager->addComponent(tileEntity, new CollisionComponent());
        m_manager->addComponent(tileEntity, new MapTileComponent());
	    m_manager->addComponent(tileEntity, new MovementComponent());
        GraphicsComponent* graphComp = new GraphicsComponent();
        graphComp->quad->setNrOfSprites(10.5f, 2.0f);
        graphComp->quad->setCurrentSprite(1.06f, 0.0f);
        m_manager->addComponent(tileEntity, graphComp);
}