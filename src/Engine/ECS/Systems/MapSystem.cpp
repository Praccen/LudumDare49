#include "MapSystem.hpp"
#include "Objects/InstancedQuadManager.hpp"
#include "ECS/ECSManager.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/CollisionComponent.hpp"
#include "ECS/Components/GraphicsComponent.hpp"

#include <cstdlib>
#include <ctime>

MapSystem::MapSystem(ECSManager *ECSManager) : System(ECSManager, ComponentTypeEnum::MAPTILE) {
    // Init random seed
    srand(static_cast<unsigned int>(time(NULL)));
}

void MapSystem::initialize() {
    m_render =  &Rendering::getInstance();
    // Create tile and platform enitites.
    for(unsigned int i = 0; i < m_numTiles; ++i) {
        createNewTile(static_cast<float>(i), 1.0f, MapTileComponent::GROUND, 1.0f);
    }
}

void MapSystem::update(float dt) {


    for(auto& e : m_entities) {
		PositionComponent *p = static_cast<PositionComponent *>(e->getComponent(ComponentTypeEnum::POSITION));
		MovementComponent *m = static_cast<MovementComponent *>(e->getComponent(ComponentTypeEnum::MOVEMENT));
		MapTileComponent *mt = static_cast<MapTileComponent *>(e->getComponent(ComponentTypeEnum::MAPTILE));

        float camX = m_render->getCamera()->getPosition().x;
        if((camX - p->position.x) > 14.f) {
            p->position.x = static_cast<float>(m_numTiles);
            if (mt->type) {
                m_manager->removeEntity(e->getID());
            }
            // Check for platform
            if(!m_isPlatform ) {
                int platform = rand() % 100 + 1;
                // 10% chance for platform
                if(platform < 10 && m_drawnTiles > 1) {
                    m_platformHeight = rand() % 3 + 3;
                    m_lastTileY += m_platformHeight;
                    m_isPlatform = true;
                    m_drawnTiles = 0;
                }
                // Random tile y value
                if (m_lastTileY < m_destHeight) {
                    p->position.y = m_lastTileY+0.1f;
                } else {
                    p->position.y = m_lastTileY-0.1f;
                }
            } else {
                if(m_numPlatformsDrawn > 5 && m_drawnTiles > 1) {
                    m_lastTileY -= m_platformHeight;
                    m_isPlatform = false;
                    m_numPlatformsDrawn = 0;
                    m_drawnTiles = 0;
                }
                m_numPlatformsDrawn++;
                // Random tile y value compensate for platform height
                if (m_lastTileY-5 < m_destHeight) {
                    p->position.y = m_lastTileY+0.1f;
                } else {
                    p->position.y = m_lastTileY-0.1f;
                }
            }

            // New destination height for ground
            if(std::fabs(m_destHeight - p->position.y) < 0.01f ) {
                m_destHeight = rand() % 4 - 2;
            }

            
            spawnObstacle();
            // Random spawning tile 20%
            int spawn = rand() % 100 + 1;
            if ((spawn < 20) && (m_drawnTiles > 1)) {
                int gap = rand() % 4+2;
                m_numTiles += gap;
                m_drawnTiles = 0;
            } else {
                m_drawnTiles++;
            }

            // Update last tile and enitity properties.
            m_lastTileY = p->position.y;
            m->constantAcceleration.y = 0.0f;
            m->velocity.y = 0.0f;
            m_numTiles++;
        } else if((camX - p->position.x) > 10.f) {
            m->constantAcceleration.y = -9.82f;
            
        } 
    }
}

void MapSystem::createNewTile(float x, float y, MapTileComponent::TILE_TYPE t, float scale) {
        Entity& tileEntity = m_manager->createEntity();
        PositionComponent* posComp = new PositionComponent(x, y);
        CollisionComponent* collisionComp = new CollisionComponent();
        GraphicsComponent* graphComp = new GraphicsComponent();
        graphComp->quad->setTexureIndex(2);
        graphComp->quad->setNrOfSprites(1.0f, 1.0f);
        graphComp->quad->setCurrentSprite(0.0f, 0.0f);

        collisionComp->isConstraint = true;
        graphComp->quad->setNrOfSprites(10.5f, 2.0f);
        graphComp->quad->setCurrentSprite(1.06f, 0.0f);
        posComp->scale = glm::vec3(scale);
        
        m_manager->addComponent(tileEntity, posComp);
        m_manager->addComponent(tileEntity, graphComp);
        m_manager->addComponent(tileEntity, collisionComp);
        m_manager->addComponent(tileEntity, new MapTileComponent(t));
	    m_manager->addComponent(tileEntity, new MovementComponent());
}

void MapSystem::spawnObstacle() {
    unsigned int spawn = rand() % 100 + 1;
    if(spawn < 5) {
        createNewTile(static_cast<float>(m_numTiles), m_lastTileY + 1, MapTileComponent::OBSTACLE, 0.5f);
    }
}