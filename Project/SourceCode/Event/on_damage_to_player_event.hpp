#pragma once

/// @brief ダメージを受けた
struct OnDamageToPlayerEvent
{
	float	damage			= 0.0f;
	float	damage_parcent	= 0.0f;		// 最大HPに対するダメージの割合(damage / max_health)
	VECTOR	hit_pos			= {};
};
