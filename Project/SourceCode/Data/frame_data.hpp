#pragma once
#include <DxLib.h>

struct FrameData
{
	MATRIX world_m		= MGetIdent();
	MATRIX local_m		= MGetIdent();

	MATRIX world_rot_m	= MGetIdent();
	MATRIX local_rot_m	= MGetIdent();

	VECTOR world_pos	= {};
	VECTOR local_pos	= {};
};
