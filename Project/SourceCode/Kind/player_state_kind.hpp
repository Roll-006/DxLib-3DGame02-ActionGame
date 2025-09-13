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
		kGrabbed,
		kDead,
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

	enum class SpecialStateKind
	{
		kSpecialNull,
		kRoundhouseKick,
		kFrontKick,
		kSuplex,
		kEscape,
		kStealthKill,
		kFinishOff,
		kEscapeWithKnife,
	};
}
