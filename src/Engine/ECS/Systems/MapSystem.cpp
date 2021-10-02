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
    srand(static_cast<unsigned int>(time(NULL)));
}

void MapSystem::initialize() {
    m_render =  &Rendering::getInstance();
    // Create tile and platform enitites.
    for(unsigned int i = 0; i < m_numTiles; ++i) {
        createNewTile(static_cast<float>(i), 1.0f);
    }
}

void MapSystem::update(float dt) {
    for(auto& e : m_entities) {
		PositionComponent *p = static_cast<PositionComponent *>(e->getComponent(ComponentTypeEnum::POSITION));
		MovementComponent *m = static_cast<MovementComponent *>(e->getComponent(ComponentTypeEnum::MOVEMENT));
        MapTileComponent *tile = static_cast<MapTileComponent*>(e->getComponent(ComponentTypeEnum::MAPTILE));
        float camX = m_render->getCamera()->getPosition().x;

        //Check if tile should be move to the front
        if((camX - p->position.x) > 14.f) {
            p->position.x = static_cast<float>(m_numTiles);

            //10% chance of getting unstable
            int unstableChance = rand() % 100 + 1;
            if (unstableChance < m_unstableChance) {
                tile->unstable = true;
            }

            if(!m_isPlatform ) {
                int platform = rand() % 100 + 1;
                if(platform < 10 && m_drawnTiles > 1) {
                    m_platformHeight = rand() % 3 + 5;
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

            // Random spawning tile or not
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
            //tile is at its end, let it fall and remov unstable
            m->constantAcceleration.y = -9.82f;
            tile->unstable = false;
            
        } 

        //gives unstable a chance to rise or fall and then remove unstable
        if (tile->unstable) {
            int activate = rand() % 100 + 1;
            if (activate < 30) {
                m->constantAcceleration.y = 1.0f;
            }
            else if (activate < 60) {
                m->constantAcceleration.y = -9.82f;  
            }
            tile->unstable = false;   
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