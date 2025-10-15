#pragma once
#include "../Kind/scene_kind.hpp"

/// @brief ƒV[ƒ“‚ª•ÏX‚³‚ê‚½
struct ChangeSceneEvent
{
	SceneKind prev_scene_kind = SceneKind::kNone;
};
