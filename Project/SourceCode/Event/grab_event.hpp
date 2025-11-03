#pragma once
#include "../Part/modeler.hpp"

/// @brief ’Í‚ñ‚¾
struct GrabEvent
{
	std::string				 enemy_handle = "";
	int						 obj_handle   = -1;
	std::shared_ptr<Modeler> modeler	  = nullptr;
};
