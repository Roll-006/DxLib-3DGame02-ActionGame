#pragma once
#include <DxLib.h>

struct HumanoidArmMatrixData
{
	MATRIX left_arm			= MGetIdent();
	MATRIX left_fore_arm	= MGetIdent();
	MATRIX left_hand		= MGetIdent();

	MATRIX right_arm		= MGetIdent();
	MATRIX right_fore_arm	= MGetIdent();
	MATRIX right_hand		= MGetIdent();
};
