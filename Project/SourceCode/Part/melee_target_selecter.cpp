#include "melee_target_selecter.hpp"

void MeleeTargetSelecter::SelectMeleeTarget(const VECTOR& forward, std::vector<MeleeCandidateData>& melee_candidate)
{
	if (melee_candidate.empty()) { return; }

	std::vector<MeleeCandidateData> distance;
	std::vector<MeleeCandidateData> angle;

	// MEMO : 画面中央に近い位置にいる場合は、カメラに距離が近い順にソート
	//		  そうでない場合は、カメラのforwardに近い順でソート
	//		  最終的に最も優先度が高いメレー対象のみを残す
	for (const auto& candidate : melee_candidate)
	{
		if (candidate.camera_diff_angle < kPrioritySwitchAngle * math::kDegToRad)
		{
			distance.emplace_back(candidate);
		}
		else
		{
			angle.emplace_back(candidate);
		}
	}

	// 距離でソート
	if (!distance.empty())
	{
		std::ranges::sort(distance,	{}, &MeleeCandidateData::distance_to_camera);
		melee_candidate = std::vector<MeleeCandidateData>{ distance.front() };
		return;
	}

	// 角度でソート
	std::ranges::sort(angle, {}, &MeleeCandidateData::camera_diff_angle);
	melee_candidate = std::vector<MeleeCandidateData>{ angle.front() };
}
