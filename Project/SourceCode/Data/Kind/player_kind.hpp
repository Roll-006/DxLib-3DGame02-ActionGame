#pragma once

enum class PlayerAnimKind
{
	kNone = -1,

	kIdle01,
	kIdle02,
	kIdleCrouch01,
	kIdleCrouchShoot01,
	kIdleShoot01,

	kWalkCrouchForward01,		// しゃがみ状態で正面を向いて前方に歩く
	kWalkCrouchBackward01,		// しゃがみ状態で正面を向いて後方に歩く
	kWalkCrouchLeft01,			// しゃがみ状態で正面を向いて左に歩く
	kWalkCrouchRight01,			// しゃがみ状態で正面を向いて右に歩く
	kWalkCrouchForwardLeft01,	// しゃがみ状態で正面を向いて左前方に歩く
	kWalkCrouchForwardRight01,	// しゃがみ状態で正面を向いて右前方に歩く
	kWalkCrouchBackwardLeft01,	// しゃがみ状態で正面を向いて左後方に歩く
	kWalkCrouchBackwardRight01,	// しゃがみ状態で正面を向いて右後方に歩く
	
	kWalkShootForward01,		// 武器を構えながら正面を向いて前方に歩く
	kWalkShootBackward01,		// 武器を構えながら正面を向いて後方に歩く
	kWalkShootLeft01,			// 武器を構えながら正面を向いて左に歩く
	kWalkShootRight01,			// 武器を構えながら正面を向いて右に歩く
	kWalkShootForwardLeft01,	// 武器を構えながら正面を向いて左前方に歩く
	kWalkShootForwardRight01,	// 武器を構えながら正面を向いて右前方に歩く
	kWalkShootBackwardLeft01,	// 武器を構えながら正面を向いて左後方に歩く
	kWalkShootBackwardRight01,	// 武器を構えながら正面を向いて右後方に歩く
	
	kRunForward01,				// 進行方向を向いて走る
};