#pragma once
#include "../Kind/scene_kind.hpp"

/// @brief シーンが変更された
struct ChangeSceneEvent
{
	SceneKind current_scene_kind	= SceneKind::kNone;		// 現在のシーン
	SceneKind next_scene_kind		= SceneKind::kNone;		// 移行先シーン
};
