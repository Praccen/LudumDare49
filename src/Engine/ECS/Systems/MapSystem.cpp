#include "MapSystem.hpp"
#include "Objects/InstancedQuadManager.hpp"
#include "ECS/ECSManager.hpp"
#include "ECS/Components/PositionComponent.hpp"
#include "ECS/Components/CollisionComponent.hpp"
#include "ECS/Components/GraphicsComponent.hpp"

MapSystem::MapSystem(ECSManager *ECSManager) : System(ECSManager, ComponentTypeEnum::POSITION) {}

void MapSystem::initialize() {
    for(unsigned int i = 0; i < m_numTiles; ++i) {
        Entity& tileEntity = m_manager->createEntity();
        m_manager->addComponent(tileEntity, new PositionComponent(static_cast<float>(i), 1.0f));
        m_manager->addComponent(tileEntity, new CollisionComponent());
	    m_manager->addComponent(tileEntity, new MovementComponent());
        GraphicsComponent* graphComp = new GraphicsComponent();
        graphComp->quad->setNrOfSprites(1.0f, 1.5f);
        graphComp->quad->setCurrentSprite(0.0f, 0.0f);
        m_manager->addComponent(tileEntity, graphComp);
    }
}

void MapSystem::update(float dt) {}