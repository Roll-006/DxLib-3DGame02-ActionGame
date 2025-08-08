#pragma once
#include "../../Part/transform.hpp"

/// @brief ’eŠÛ‚ª”­ŽË‚³‚ê‚½
struct OnShotBulletData
{
	std::string					bullet_name			= "";
	int							m_obj_handle		= -1;
	std::shared_ptr<Transform>	bullet_transform	= nullptr;
};
