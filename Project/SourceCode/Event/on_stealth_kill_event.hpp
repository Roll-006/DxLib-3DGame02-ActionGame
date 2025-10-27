#pragma once
#include "../Part/modeler.hpp"

/// @brief ステルスキルされた
struct OnStealthKillEvent
{
	int enemy_handle = -1;
	std::shared_ptr<Modeler> modeler = nullptr;
};
