#pragma once
#include "Component.hpp"
#include <vector>

enum class PowerUpType {
    Hen
};
class PowerUpComponent : public Component {
public:
    PowerUpComponent();
    std::vector<PowerUpType> type;

};

