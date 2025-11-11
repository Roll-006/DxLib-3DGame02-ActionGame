#pragma once

namespace boss_state
{
	enum class AIStateKind
	{
		kWait,					// 待機
		kPatrol,				// 巡回(特定のルートを移動する)
		kInvestigate,			// 調査(対象を見失った場所を調査する)
		kSearch,				// 捜索(調査で対象を発見できなかった際、さらに広範囲を捜索する)
		kAlert,					// 警戒(音や気配に警戒して周りを見渡す)
		kTrack,					// 追跡(対象を追尾する)
		kRunAttack,				// ダッシュ攻撃
		kCloseRangeAttack,		// 近接攻撃
		kLongRangeAttack,		// 遠距離攻撃
	};

	enum class MoveStateKind
	{
		kMoveNull,
		kMove,
	};

	enum class ActionStateKind
	{
		kActionNull,			// NULL
		kDetected,				// 発見
		kRun,					// ダッシュ
		kStealthKilled,			// ステルスキルされた
		kKnockback,				// ノックバック
		kBackwardKnockback,		// ノックバック(後ろ)
		kStandUp,				// 立ち上がる
		kStandStun,				// 怯み(立ち状態)
		kCrouchLeftStun,		// 怯み(左足しゃがみ状態)
		kCrouchRightStun,		// 怯み(右足しゃがみ状態)
		kDead,					// 死亡
	};
}
