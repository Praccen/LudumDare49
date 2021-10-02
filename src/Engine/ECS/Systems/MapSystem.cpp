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
		MovementComponent *m = static_cast<MovementComponent *>(e->getComponent(ComponentTypeEnum::MOVEMENT));
        float camX = render->getCamera()->getPosition().x;
        if((camX - p->position.x) > 14.f) {
            p->position.x = static_cast<float>(m_numTiles);
            p->position.y = 1.0f;
            m->constantAcceleration.y = 0.0f;
            m->velocity.y = 0.0f;
            m_numTiles++;
        } else if((camX - p->position.x) > 10.f) {
            m->constantAcceleration.y = -9.82f;
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