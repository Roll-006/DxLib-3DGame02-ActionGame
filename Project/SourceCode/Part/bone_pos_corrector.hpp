#pragma once
#include <string>
#include <unordered_map>
#include <DxLib.h>

#include "../Data/Kind/time_kind.hpp"
#include "../Data/Path/bone_path.hpp"
#include "../Calculation/math.hpp"

class BonePosCorrector
{
public:
	/// @brief 銃を構えた際のボーン位置を修正
	/// @param model_handle モデルハンドル
	/// @param rot ターゲットを見た時の姿勢
	/// @param m_is_ready_gun 銃を構えているか
	void CorrectGunPoseBone(const int model_handle, const VECTOR& look_dir, const MATRIX& rot, const bool m_is_ready_gun);

private:
	std::unordered_map<std::string, std::unordered_map<TimeKind, VECTOR>> m_bone_angle;
};
