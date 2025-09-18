#pragma once
#include "../Part/transform.hpp"

/// @brief ’eŠÛ‚ª”­ŽË‚³‚ê‚½
struct OnShotBulletEvent
{
	std::string					bullet_name			= "";
	std::string					weapon_owner_name	= "";
	int							obj_handle			= -1;
	std::shared_ptr<Transform>	bullet_transform	= nullptr;
};
