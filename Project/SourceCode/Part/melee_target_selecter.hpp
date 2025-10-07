#pragma once
#include <ranges>
#include <vector>
#include <DxLib.h>

#include "../Interface/i_melee_attackable.hpp"
#include "../Calculation/math.hpp"

class MeleeTargetSelecter final
{
public:
	void SelectMeleeTarget(const VECTOR& forward, std::shared_ptr<IMeleeAttackable>& melee_attacker);

private:
	static constexpr float kPrioritySwitchAngle = 30.0f;
};
