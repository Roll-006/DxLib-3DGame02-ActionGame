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
		kKnifeEquipped,
		kAimKnife,
		kStabKnife,
		kSideSlashKnife,
		kParry,
		kGunEquipped,
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
