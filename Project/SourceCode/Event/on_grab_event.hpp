#pragma once
#include "../Part/modeler.hpp"

/// @brief �͂܂ꂽ
struct OnGrabEvent
{
	std::shared_ptr<Modeler> modeler = nullptr;
	int obj_handle = -1;
};
