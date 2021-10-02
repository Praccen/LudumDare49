#include "Component.hpp"

#include "Objects/Quad.hpp"

class GraphicsComponent : public Component 
{

public:
	Quad* quad;

	glm::vec2 startingTile;
	glm::vec2 advanceBy;
	float updateInterval;
	glm::vec2 modAdvancement;
	float updateTimer;

	GraphicsComponent();
	virtual ~GraphicsComponent();
};