#pragma once

namespace zombie_state
{
	enum class ActionStateKind
	{
		kIdle,			// IDLE
		kPatrol,		// „‰ñ
		kInvestigate,	// ’²¸
		kSearch,		// ‘{õ
		kAlert,			// Œx‰ú
		kTrack,			// ’ÇÕ
		kGrab,			// ’Í‚İ
		kStun,			// ƒ_ƒEƒ“
		kPlayDead,		// €‚ñ‚¾‚Ó‚è
		kDead,			// €–S
	};
}
