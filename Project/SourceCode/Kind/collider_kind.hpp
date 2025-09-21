#pragma once

/// @brief コライダーおよびトリガーの種類
enum class ColliderKind
{
	kNone = -1,

	kCollider,				// コライダー(押し戻し用)
	kMeshTrigger,			// モデルが持つメッシュをトリガーに利用
	kRayCast,				// レイキャスト

	kAttackTrigger,			// 攻撃判定用トリガー
	kLandingTrigger,		// 着地判定用トリガー
	kVisionTrigger,			// 視界判定用トリガー

	kReactionTrigger,		// リアクショントリガー

	kHeadTrigger,			// 頭部トリガー
	kUpBodyTrigger,			// 胴体の上側トリガー
	kDownBodyTrigger,		// 胴体の下側トリガー
	kLeftUpperArmTrigger,	// 左上腕トリガー
	kLeftForearmTrigger,	// 左前腕トリガー
	kLeftHandTrigger,		// 左手トリガー
	kRightUpperArmTrigger,	// 右上腕トリガー
	kRightForearmTrigger,	// 右前腕トリガー
	kRightHandTrigger,		// 右手トリガー
	kLeftUpLegTrigger,		// 左脚の上側(太もも部分)トリガー
	kRightUpLegTrigger,		// 右脚の上側(太もも部分)トリガー
	kLeftDownLegTrigger,	// 左脚の下側(スネ・ふくらはぎ部分)トリガー
	kRightDownLegTrigger,	// 右脚の下側(スネ・ふくらはぎ部分)トリガー
};
