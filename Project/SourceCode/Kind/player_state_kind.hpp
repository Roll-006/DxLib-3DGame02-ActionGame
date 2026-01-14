#pragma once

enum class PlayerStateKind
{
	kNone = -1,

	kIdle,
	kDead,

	kAttachWeapon,
	kDetachWeapon,
	kAimGun,
	kAimKnife,
	kEquipGun,
	kEquipKnife,
	kReload,
	kShot,
	kShotRocketLauncher,

	kFirstSideSlashKnife,
	kSecondSideSlashKnife,
	kSpinningSlashKnife,
	kFrontKick,
	kRoundhouseKick,

	kEscape,
	kGrabbed,
	kStealthKill,
	kVictoryPose,
};
