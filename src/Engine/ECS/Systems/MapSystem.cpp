#include "MapSystem.hpp"
#include "Objects/InstancedQuadManager.hpp"
#include "ECS/ECSManager.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/CollisionComponent.hpp"
#include "ECS/Components/GraphicsComponent.hpp"
#include "ECS/Components/MapTileComponent.hpp"

#include <cstdlib>
#include <ctime>

MapSystem::MapSystem(ECSManager *ECSManager) : System(ECSManager, ComponentTypeEnum::MAPTILE) {
    // Init random seed
    srand(time(NULL));
}

void MapSystem::initialize() {
    m_render =  &Rendering::getInstance();
    for(unsigned int i = 0; i < m_numTiles; ++i) {
        createNewTile(static_cast<float>(i), 1.0f);
    }
}

void MapSystem::update(float dt) {

    for(auto& e : m_entities) {
		PositionComponent *p = static_cast<PositionComponent *>(e->getComponent(ComponentTypeEnum::POSITION));
		MovementComponent *m = static_cast<MovementComponent *>(e->getComponent(ComponentTypeEnum::MOVEMENT));
        float camX = m_render->getCamera()->getPosition().x;
        if((camX - p->position.x) > 14.f) {
            p->position.x = static_cast<float>(m_numTiles);
            int upDown = rand() % 3;
            if (upDown == 0) {
                p->position.y = lastTileY-0.1f;
            } else if (upDown == 1){
                p->position.y = lastTileY+0.1f;
            } else {
                p->position.y = lastTileY;
            }

            int spawn = rand() % 100 + 1;
            if (spawn < 20) {
                int gap = rand() % 4+2;
                m_numTiles += gap;
            }

            lastTileY = p->position.y;
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
        CollisionComponent* collisionComp = new CollisionComponent();
        collisionComp->isConstraint = true;
        m_manager->addComponent(tileEntity, collisionComp);
        m_manager->addComponent(tileEntity, new MapTileComponent());
	    m_manager->addComponent(tileEntity, new MovementComponent());
        GraphicsComponent* graphComp = new GraphicsComponent();
        graphComp->quad->setNrOfSprites(10.5f, 2.0f);
        graphComp->quad->setCurrentSprite(1.06f, 0.0f);
        m_manager->addComponent(tileEntity, graphComp);
}