#pragma once

#include "ShaderPrograms/ShaderProgram.hpp"

class BlurEffect : public ShaderProgram {
public:
    BlurEffect();
    ~BlurEffect() = default;
	void setupVertexAttributePointers() override;
};