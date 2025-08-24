#pragma once

namespace zombie_state
{
	enum class ActionStateKind
	{
		kActionNull,	// IDLE
		kPatrol,		// „‰ñ
		kInvestigate,	// ’²¸
		kSearch,		// ‘{õ
		kAlert,			// Œx‰ú
		kTrack,			// ’ÇÕ
		kGrab,			// ’Í‚İ
		kStandStun,		// ‹¯‚İ(—§‚¿ó‘Ô)
		kCrouchStun,	// ‹¯‚İ(‚µ‚á‚ª‚İó‘Ô)
		kPlayDead,		// €‚ñ‚¾‚Ó‚è
		kDead,			// €–S
	};
}
