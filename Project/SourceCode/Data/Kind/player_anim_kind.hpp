#pragma once

enum class PlayerAnimKind
{
	kNone = -1,


	#pragma region 汎用
	kIdle,

	kStabKnife,
	kFirstSideSlashKnife,
	kSecondSideSlashKnife,
	kSpinningSlashKnife,

	kMoveForwardRun,
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
	#pragma endregion


	#pragma region 下半身用
	kCrouch,

	kMoveForward,
	kMoveBackward,
	kMoveLeft,
	kMoveRight,
	kMoveForwardLeft,
	kMoveForwardRight,
	kMoveBackwardLeft,
	kMoveBackwardRight,

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
