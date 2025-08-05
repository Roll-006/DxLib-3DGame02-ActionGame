#pragma once

enum class PlayerAnimKind
{
	kNone = -1,


	#pragma region UPPER BODY
	kEquipKnife,
	kAimKnife,
	kStabKnife,
	kFirstSideSlashKnife,
	kSecondSideSlashKnife,
	kSpinningSlashKnife,
	kParry,
	kEquipGun,
	kAimGun,
	kReload,
	#pragma endregion


	#pragma region LOWER BODY
	kIdle,
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

	kMoveForwardRun,
	#pragma endregion
};
