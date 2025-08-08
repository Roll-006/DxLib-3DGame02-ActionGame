#pragma once
#include "../Kind/gun_kind.hpp"
#include "../../Part/transform.hpp"

/// @brief ロケットランチャーが弾丸を発射した
struct RocketLauncherShotData
{
	std::shared_ptr<Transform>	m_ejection_port_transform = nullptr;
};
