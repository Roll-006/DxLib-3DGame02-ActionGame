#pragma once
#include "../Calculation/matrix.hpp"
#include "../Data/frame_data.hpp"

namespace frame_info
{
	[[nodiscard]] inline const FrameData GetFrameInfo(const int model_handle, const int frame_index)
	{
		auto	   world_m		= MV1GetFrameLocalWorldMatrix	(model_handle, frame_index);
		auto	   local_m		= MV1GetFrameLocalMatrix		(model_handle, frame_index);
		const auto world_rot_m	= matrix::GetRotMatrix			(world_m);
		const auto local_rot_m	= matrix::GetRotMatrix			(local_m);
		const auto world_pos	= matrix::GetPos				(world_m);
		const auto local_pos	= matrix::GetPos				(local_m);

		return FrameData(world_m, local_m, world_rot_m, local_rot_m, world_pos, local_pos);
	}
}
