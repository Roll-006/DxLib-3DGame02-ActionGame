#pragma once

enum class ColliderKind
{
	kCollider,			// コライダー(押し戻し用)
	kLandingTrigger,	// 着地判定用トリガー
	kHeadTrigger,		// 頭部トリガー
};
