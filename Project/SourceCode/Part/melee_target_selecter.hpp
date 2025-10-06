#pragma once
#include <ranges>
#include <vector>
#include <DxLib.h>

#include "../Data/melee_candidate_data.hpp"
#include "../Calculation/math.hpp"

class MeleeTargetSelecter final
{
public:
	void SelectMeleeTarget(const VECTOR& forward, std::vector<MeleeCandidateData>& melee_candidate);

private:
	static constexpr float kPrioritySwitchAngle = 30.0f;
};
