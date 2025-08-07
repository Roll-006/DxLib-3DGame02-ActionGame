#pragma once

enum class EventKind
{
	kShot,					// 銃を撃った
	kHitBulletToObj,		// 弾丸がオブジェクトに衝突した
	kHitBulletToEnemy,		// 弾丸が敵に衝突した
	kShotRocketLauncher,	// ロケットランチャーが発射された
};
