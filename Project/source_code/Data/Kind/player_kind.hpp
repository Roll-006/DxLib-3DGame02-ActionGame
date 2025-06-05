#pragma once

enum class PlayerAnimKind
{
	kNone = -1,

	kIdle01,
	kIdleSquat01,

	kWalkSquatForward01,		// しゃがみ状態で正面を向いて前方に歩く
	kWalkSquatBackward01,		// しゃがみ状態で正面を向いて後方に歩く
	kWalkSquatLeft01,			// しゃがみ状態で正面を向いて左に歩く
	kWalkSquatRight01,			// しゃがみ状態で正面を向いて右に歩く
	kWalkSquatForwardLeft01,	// しゃがみ状態で正面を向いて左前方に歩く
	kWalkSquatForwardRight01,	// しゃがみ状態で正面を向いて右前方に歩く
	kWalkSquatBackwardLeft01,	// しゃがみ状態で正面を向いて左後方に歩く
	kWalkSquatBackwardRight01,	// しゃがみ状態で正面を向いて右後方に歩く
	
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