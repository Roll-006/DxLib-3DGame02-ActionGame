#pragma once

namespace player_state
{
	enum class MoveStateKind
	{
		kNone = -1,

		kIdle,
		kMove,
	};

	enum class ActionStateKind
	{
		kNone = -1,

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
		kEscape,
		kStealthKill,
		kFinishOff,
		kEscapeWithKnife,
		kVictoryPose,
	};

	enum class WeaponActionStateKind
	{
		kNone = -1,

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
