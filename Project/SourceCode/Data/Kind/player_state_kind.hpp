#pragma once

namespace player_state
{
	enum class MoveStateKind
	{
		kMoveNull,
		kMove,
	};

	enum class ActionStateKind
	{
		kActionNull,
		kCrouch,
		kRun,
		kTurnAround,
		kCrouchTurnAround,
	};

	enum class WeaponActionStateKind
	{
		kWeaponActionNull,
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
