#pragma once

#include "ShaderPrograms/ShaderProgram.hpp"

class BlurEffect : public ShaderProgram {
public:

    BlurEffect();
	void setupVertexAttributePointers() override;
};