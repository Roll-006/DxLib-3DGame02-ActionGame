#pragma once
#include "../Kind/gun_kind.hpp"
#include "../Part/transform.hpp"

/// @brief ロケットランチャーが弾丸を発射した(必殺技)
struct RocketLauncherShotEvent
{
	std::string					weapon_owner_name		= "";
	std::shared_ptr<Transform>	ejection_port_transform = nullptr;
};
