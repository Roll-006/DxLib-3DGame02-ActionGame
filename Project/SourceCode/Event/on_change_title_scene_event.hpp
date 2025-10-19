#pragma once
#include "../Part/transform.hpp"

/// @brief タイトルシーンに遷移した
struct OnChangeTitleSceneEvent
{
	std::shared_ptr<Transform> smoke_transform;
};
