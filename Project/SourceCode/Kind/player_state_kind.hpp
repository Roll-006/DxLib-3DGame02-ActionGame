#pragma once

namespace player_state
{
	enum class MoveStateKind
	{
		kIdle,
		kMove,
	};

	enum class ActionStateKind
	{
		kActionNull,
		kCrouch,
		kRun,
		kTurnAround,
		kCrouchTurnAround,
		kDead,

		kGrabbed,
		kFrontKick,
		kRoundhouseKick,
		kSuplex,
		kEscape,		// TODO : çÌèúåÛï‚ÅBGrabbedÇ…àœÇÀÇÈ
		kStealthKill,
		kFinishOff,
		kEscapeWithKnife,
	};

	enum class WeaponActionStateKind
	{
		kWeaponActionNull,
		kAttachWeapon,
		kDetachWeapon,
		kEquipKnife,
		kAimKnife,
		kStabKnife,
		kFirstSideSlashKnife,
		kSecondSideSlashKnife,
		kSpinningSlashKnife,
		kParry,
		kEquipGun,
		kAimGun,
		kShot,
		kShotRocketLauncher,
		kReload,
	};
}
