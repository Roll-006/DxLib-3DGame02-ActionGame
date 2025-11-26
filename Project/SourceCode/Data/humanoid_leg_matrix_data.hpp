#pragma once
#include <DxLib.h>

struct HumanoidLegMatrixData
{
	MATRIX armature			= MGetIdent();
	MATRIX hips				= MGetIdent();

	MATRIX left_up_leg		= MGetIdent();
	MATRIX left_leg			= MGetIdent();
	MATRIX left_foot		= MGetIdent();
	MATRIX left_toe_base	= MGetIdent();
	MATRIX left_toe_end		= MGetIdent();

	MATRIX right_up_leg		= MGetIdent();
	MATRIX right_leg		= MGetIdent();
	MATRIX right_foot		= MGetIdent();
	MATRIX right_toe_base	= MGetIdent();
	MATRIX right_toe_end	= MGetIdent();
};
