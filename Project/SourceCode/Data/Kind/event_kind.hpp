#pragma once

enum class EventKind
{
	kWeaponShot,					// 銃が弾丸を発射した
	kRocketLauncherShot,			// ロケットランチャーが弾丸を発射した
	kOnShotBullet,					// 弾丸が発射された
	kOnHitBullet,					// 弾丸が衝突した
	kEndRocketLauncherCutscene,		// ロケットランチャー発射時の演出が終了した
};
