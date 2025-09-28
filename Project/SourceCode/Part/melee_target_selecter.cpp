#include "melee_target_selecter.hpp"

void MeleeTargetSelecter::SelectMeleeTarget(const VECTOR& forward, std::vector<MeleeCandidateData>& melee_candidate)
{
	if (melee_candidate.empty()) { return; }

	// TODO : 仮で先頭のみを残す。のちに変更。
	melee_candidate = std::vector<MeleeCandidateData>{ melee_candidate.front() };
}
