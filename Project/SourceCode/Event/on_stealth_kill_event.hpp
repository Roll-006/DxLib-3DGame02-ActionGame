#pragma once
#include "../Part/modeler.hpp"

/// @brief ステルスキルされた
struct OnStealthKillEvent
{
	std::string				 enemy_id = "";
	std::shared_ptr<Modeler> modeler  = nullptr;
};
