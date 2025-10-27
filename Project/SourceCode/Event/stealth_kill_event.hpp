#pragma once
#include "../Part/modeler.hpp"

/// @brief ステルスキルした
struct StealthKillEvent
{
	std::shared_ptr<Modeler> modeler = nullptr;
};
