#pragma once
#include "Component.hpp"

class PlayerComponent : public Component {

public:
	bool alive;
	unsigned int score;
	PlayerComponent();
};