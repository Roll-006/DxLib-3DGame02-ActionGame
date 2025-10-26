#pragma once

struct StealthKillCandidateData
{
	int	  target_obj_handle = -1;
	float camera_diff_angle = 0.0f;		// ƒJƒƒ‰‚Ìforward‚©‚ç‚ÌŠp“x‚Ì·
	float distance_to_camera = 0.0f;
};
