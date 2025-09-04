#pragma once

namespace zombie_state
{
	enum class AIStateKind
	{
		kWait,			// 待機
		kPatrol,		// 巡回
		kInvestigate,	// 調査
		kSearch,		// 捜索
		kAlert,			// 警戒
		kTrack,			// 追跡
		kBattle,		// 戦闘
	};

	enum class MoveStateKind
	{
		kMoveNull,
		kMove,
	};

	enum class ActionStateKind
	{
		kActionNull,	// NULL
		kRun,			// ダッシュ
		kGrab,			// 掴み
		kGrabRun,		// 掴みダッシュ
		kKnockback,		// ノックバック
		kStandStun,		// 怯み(立ち状態)
		kCrouchStun,	// 怯み(しゃがみ状態)
		kPlayDead,		// 死んだふり
		kDead,			// 死亡
	};
}
