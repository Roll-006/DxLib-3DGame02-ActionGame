#pragma once
#include "../Kind/gun_kind.hpp"
#include "../Kind/time_scale_layer_kind.hpp"
#include "../Part/transform.hpp"

/// @brief •Ší‚ª’eŠÛ‚ğ”­Ë‚µ‚½
struct WeaponShotEvent
{
	GunKind						gun_kind				= static_cast<GunKind>(0);
	std::string					weapon_owner_name		= "";
	std::shared_ptr<Transform>	muzzle_transform		= nullptr;
	std::shared_ptr<Transform>	ejection_port_transform	= nullptr;
	TimeScaleLayerKind			time_scale_layer_kind	= TimeScaleLayerKind::kNoneScale;
};
