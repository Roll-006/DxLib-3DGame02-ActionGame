#pragma once

namespace zombie_state
{
	enum class ActionStateKind
	{
		kActionNull,	// IDLE
		kWalk,			// •à‚«
		kRun,			// ƒ_ƒbƒVƒ…
		kGrab,			// ’Í‚İ
		kStandStun,		// ‹¯‚İ(—§‚¿ó‘Ô)
		kCrouchStun,	// ‹¯‚İ(‚µ‚á‚ª‚İó‘Ô)
		kPlayDead,		// €‚ñ‚¾‚Ó‚è
		kDead,			// €–S
	};
}
