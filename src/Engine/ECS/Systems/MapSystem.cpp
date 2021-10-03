#include "MapSystem.hpp"
#include "Objects/InstancedQuadManager.hpp"
#include "ECS/ECSManager.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/CollisionComponent.hpp"
#include "ECS/Components/GraphicsComponent.hpp"
#include "ECS/Components/HealthComponent.hpp"

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
        createNewTile(static_cast<float>(i), 1.0f, MapTileComponent::GROUND, 1.1f, 1.1f * 0.75f);
    }
}

void MapSystem::update(float dt) {


    for(auto& e : m_entities) {
		PositionComponent *p = static_cast<PositionComponent *>(e->getComponent(ComponentTypeEnum::POSITION));
		MovementComponent *m = static_cast<MovementComponent *>(e->getComponent(ComponentTypeEnum::MOVEMENT));
		MapTileComponent *mt = static_cast<MapTileComponent *>(e->getComponent(ComponentTypeEnum::MAPTILE));

        float camX = m_render->getCamera()->getPosition().x;

        if (mt->destroyed) {
            m_manager->removeComponent(*e, ComponentTypeEnum::GRAPHICS);
            m_manager->removeComponent(*e, ComponentTypeEnum::COLLISION);
            mt->wasDestroyed = true;
            mt->destroyed = false;
            continue;
        }

        //Check if tile should be move to the front
        if((camX - p->position.x) > 14.f) {
            //if tile was destroyed, re-add components and reset life
            if (mt->wasDestroyed) {
                mt->wasDestroyed = false;
                HealthComponent* h = static_cast<HealthComponent*>(e->getComponent(ComponentTypeEnum::HEALTH));
                h->health = h->maxHealth;

                CollisionComponent* collisionComp = new CollisionComponent();
                GraphicsComponent* graphComp = new GraphicsComponent();
                graphComp->quad->setTextureIndex(2);
                graphComp->quad->setNrOfSprites(1.0f, 1.0f);
                graphComp->quad->setCurrentSprite(0.0f, 0.0f);
                collisionComp->isConstraint = true;

                m_manager->addComponent(*e, graphComp);
                m_manager->addComponent(*e, collisionComp);
            }

            p->position.x = static_cast<float>(m_numTiles);

            //10% chance of getting unstable
            int unstableChance = rand() % 100 + 1;
            if (unstableChance < m_unstableChance) {
                mt->unstable = true;
            }

            if (mt->type) {
                m_manager->removeEntity(e->getID());
            }
            // Check for platform
            if(!m_isPlatform ) {
                int platform = rand() % 100 + 1;
                // check if to spawn platform
                if(platform < m_platformChance && m_drawnTiles > 1) {
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

            // Spawn an obstacle
            spawnObstacle();

            // Random spawning tile 
            int spawn = rand() % 100 + 1;
            if ((spawn < m_obstacleChance) && (m_drawnTiles > 1)) {
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
            mt->unstable = false;
        } 

        //gives unstable a chance to rise or fall and then remove unstable
        if (mt->unstable) {
            int activate = rand() % 100 + 1;
            if (activate < 50) {
                m->constantAcceleration.y = 1.0f;
            }
            else {
                m->constantAcceleration.y = -9.82f;  
            }
            mt->unstable = false;   
        }
    }

    //add to unstable timer
    m_unstableTimer += dt;
    if (m_unstableTimer > m_unstableInterval) {
        m_unstableTimer = 0.0f;
        increaseUnstable();
    }
}

void MapSystem::increaseUnstable()
{
    m_unstableChance += 1;
}

void MapSystem::createNewTile(float x, float y, MapTileComponent::TILE_TYPE t, float scaleX, float scaleY) {
        Entity& tileEntity = m_manager->createEntity();
        PositionComponent* posComp = new PositionComponent(x, y);
        CollisionComponent* collisionComp = new CollisionComponent();
        GraphicsComponent* graphComp = new GraphicsComponent();
        graphComp->quad->setTextureIndex(2);
        graphComp->quad->setNrOfSprites(1.0f, 1.0f);
        graphComp->quad->setCurrentSprite(0.0f, 0.0f);
        collisionComp->isConstraint = true;
        posComp->scale = glm::vec3(scaleX, scaleY, 1.f);
        
        m_manager->addComponent(tileEntity, posComp);
        m_manager->addComponent(tileEntity, graphComp);
        m_manager->addComponent(tileEntity, collisionComp);
        m_manager->addComponent(tileEntity, new MapTileComponent(t));
	    m_manager->addComponent(tileEntity, new MovementComponent());
        m_manager->addComponent(tileEntity, new HealthComponent(1));
}

void MapSystem::spawnObstacle() {
    unsigned int spawn = rand() % 100 + 1;
    if(spawn < 5) {
        createNewTile(static_cast<float>(m_numTiles), m_lastTileY + 1, MapTileComponent::OBSTACLE, 0.5f, 0.9f);
    }
}