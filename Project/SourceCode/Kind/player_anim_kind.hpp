#pragma once

enum class PlayerAnimKind
{
	kNone = -1,


	#pragma region 汎用
	kIdle,
	kIdleInjured,
	kStandToCrouch,
	kDead,

	kGrabbed,
	kFrontKick,
	kRoundhouseKick,
	kEscapeKick,

	kStabKnife,
	kFirstSideSlashKnife,
	kSecondSideSlashKnife,
	kSpinningSlashKnife,
	kStealthKill,

	kMoveForwardRun,
	kMoveForwardRunInjured,
	#pragma endregion


	#pragma region 上半身用
	kAttachRifle,
	kDetachRifle,
	kAttachHandgun,
	kDetachHandgun,
	kEquipKnife,
	kAimKnife,
	kParry,
	kEquipGun,
	kAimGun,
	kReload,
	kTalkingOnPhone,
	#pragma endregion


	#pragma region 下半身用
	kCrouch,
	kTalkingCrouch,

	kMoveForward,
	kMoveBackward,
	kMoveLeft,
	kMoveRight,
	kMoveForwardLeft,
	kMoveForwardRight,
	kMoveBackwardLeft,
	kMoveBackwardRight,

	kMoveForwardInjured,
	kMoveBackwardInjured,
	kMoveLeftInjured,
	kMoveRightInjured,
	kMoveForwardLeftInjured,
	kMoveForwardRightInjured,
	kMoveBackwardLeftInjured,
	kMoveBackwardRightInjured,

	kMoveForwardCrouch,
	kMoveBackwardCrouch,
	kMoveLeftCrouch,
	kMoveRightCrouch,
	kMoveForwardLeftCrouch,
	kMoveForwardRightCrouch,
	kMoveBackwardLeftCrouch,
	kMoveBackwardRightCrouch,
	#pragma endregion
};
