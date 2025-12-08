#pragma once
#include "../Part/modeler.hpp"

/// @brief ’Í‚ñ‚¾
struct GrabEvent
{
	std::string				 enemy_id	= "";
	int						 obj_handle = -1;
	std::shared_ptr<Modeler> modeler	= nullptr;
	VECTOR					 head_pos	= {};
};
