#pragma once
#include <DxLib.h>

struct HumanoidBodyMatrixData
{
	//MATRIX armature	= MGetIdent();
	//MATRIX hips		= MGetIdent();
	MATRIX spine		= MGetIdent();
	MATRIX spine1		= MGetIdent();
	MATRIX spine2		= MGetIdent();
	MATRIX neck			= MGetIdent();
	MATRIX head			= MGetIdent();

	MATRIX head_top_end	= MGetIdent();
	MATRIX left_eye		= MGetIdent();
	MATRIX right_eye	= MGetIdent();
};
