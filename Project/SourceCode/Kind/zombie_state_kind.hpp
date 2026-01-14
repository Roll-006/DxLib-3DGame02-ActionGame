#pragma once

enum class ZombieStateKind
{
	kNone = -1,

	kIdle,					// IDLE
	kDead,					// 死亡
	kPatrol,				// 巡回
	kTrack,					// 追跡
	kSearch,				// 捜索
	kStandUp,				// 立ち上がる
	kStandStun,				// 怯み(立ち状態)
	kCrouchLeftStun,		// 怯み(左足しゃがみ状態)
	kCrouchRightStun,		// 怯み(右足しゃがみ状態)
	kStealthKilled,			// ステルスキルされた
	kDetected,				// 発見
	kGrab,					// 掴み
	kGrabRun,				// 掴みダッシュ
	kKnockback,				// ノックバック
	kBackwardKnockback,		// ノックバック(後ろ)
};
