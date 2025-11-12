#pragma once
#include "../Part/modeler.hpp"

/// @brief Ÿ—˜ƒ|[ƒY‚ğ‚Æ‚Á‚½
struct VictoryPoseEvent
{
	std::shared_ptr<Transform>	transform	= nullptr;
	std::shared_ptr<Modeler>	modeler		= nullptr;
};
