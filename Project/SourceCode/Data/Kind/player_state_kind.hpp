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
	};

	enum class WeaponActionStateKind
	{
		kWeaponActionNull,
		kKnifeEquipped,
		kStabKnife,
		kSideSlashKnife,
		kParry,
		kGunEquipped,
		kAiming,
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
