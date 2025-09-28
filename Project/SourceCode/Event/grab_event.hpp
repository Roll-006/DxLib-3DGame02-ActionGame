#pragma once
#include "../Part/modeler.hpp"

/// @brief ’Í‚ñ‚¾
struct GrabEvent
{
	int enemy_handle = -1;
	std::shared_ptr<Modeler> modeler = nullptr;
};
