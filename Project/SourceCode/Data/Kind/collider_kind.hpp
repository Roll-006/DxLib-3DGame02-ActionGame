#pragma once

enum class ColliderKind
{
	kCollider,			// コライダー(押し戻し用)
	kMeshTrigger,		// モデルが持つメッシュをトリガーに利用
	kRayCast,			// レイキャスト
	kLandingTrigger,	// 着地判定用トリガー
	kHeadTrigger,		// 頭部トリガー
	kLeftLegTrigger,	// 左足トリガー
	kRightLegTrigger,	// 右足トリガー
};
