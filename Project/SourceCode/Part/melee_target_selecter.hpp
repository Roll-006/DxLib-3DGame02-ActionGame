#pragma once
#include <vector>
#include "../Data/melee_candidate_data.hpp"

class MeleeTargetSelecter final
{
public:
	void SelectMeleeTarget(const VECTOR& forward, std::vector<MeleeCandidateData>& melee_candidate);

private:

};
