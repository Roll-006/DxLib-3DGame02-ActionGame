#pragma once

enum class ColliderKind
{
	kCollider,				// コライダー(押し戻し用)
	kMeshTrigger,			// モデルが持つメッシュをトリガーに利用
	kRayCast,				// レイキャスト
	kLandingTrigger,		// 着地判定用トリガー
	kHeadTrigger,			// 頭部トリガー
	kBodyTrigger,			// 胴体トリガー
	kLeftUpLegTrigger,		// 左脚の上側(太もも部分)トリガー
	kRightUpLegTrigger,		// 右脚の上側(太もも部分)トリガー
	kLeftDownLegTrigger,	// 左脚の下側(スネ・ふくらはぎ部分)トリガー
	kRightDownLegTrigger,	// 右脚の下側(スネ・ふくらはぎ部分)トリガー
};
