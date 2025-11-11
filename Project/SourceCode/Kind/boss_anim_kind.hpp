#pragma once

enum class BossAnimKind
{
	kNone = -1,


	#pragma region 汎用
	kIdle,
	kDetected,

	kMoveForwardWalk,
	kMoveForwardRun,

	kStealthKilled,
	kDead,
	kFlyingKnockback,
	kBackwardKnockback,
	kStandUp,
	kStandUpkStandUpStealthKill,
	#pragma endregion


	#pragma region 上半身用
	kStandStun,
	#pragma endregion


	#pragma region 下半身用
	kCrouchLeftStun,
	kCrouchRightStun,
	#pragma endregion
};
