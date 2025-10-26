#include "melee_target_searcher.hpp"

MeleeTargetSearcher::MeleeTargetSearcher(std::shared_ptr<Player>& player) : 
	m_player(player)
{
	// イベントの登録
	EventSystem::GetInstance()->Subscribe<OnDownedFarEnemySpottedEvent>	(this, &MeleeTargetSearcher::AddVisibleDownedCharacter);
	EventSystem::GetInstance()->Subscribe<OnDownedNearEnemySpottedEvent>(this, &MeleeTargetSearcher::AddMeleeCandidate);
}

MeleeTargetSearcher::~MeleeTargetSearcher()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<OnDownedFarEnemySpottedEvent> (this, &MeleeTargetSearcher::AddVisibleDownedCharacter);
	EventSystem::GetInstance()->Unsubscribe<OnDownedNearEnemySpottedEvent>(this, &MeleeTargetSearcher::AddMeleeCandidate);
}

void MeleeTargetSearcher::Update()
{
	m_player->RemoveMeleeTarget();
	m_player->RemoveTopPriorityDownedChara();

	SearchMeleeTarget();
	SearchTopPriorityDownedChara();

	m_melee_candidate.clear();
	m_visible_downed_chara.clear();
}


#pragma region Event
void MeleeTargetSearcher::AddVisibleDownedCharacter(const OnDownedFarEnemySpottedEvent& event)
{
	m_visible_downed_chara.emplace_back(MeleeCandidateData(event.target_obj_handle, event.camera_diff_angle, event.distance_to_camera));
}

void MeleeTargetSearcher::AddMeleeCandidate(const OnDownedNearEnemySpottedEvent& event)
{
	m_melee_candidate.emplace_back(MeleeCandidateData(event.target_obj_handle, event.camera_diff_angle, event.distance_to_camera));
}
#pragma endregion


void MeleeTargetSearcher::SearchMeleeTarget()
{
	if (m_melee_candidate.empty() || !m_player->CanSearchMeleeTarget()) { return; }

	std::vector<MeleeCandidateData> distance;
	std::vector<MeleeCandidateData> angle;

	// MEMO : 画面中央に近い位置にいる場合は、カメラに距離が近い順にソート
	//		  そうでない場合は、カメラのforwardに近い順でソート
	//		  最終的に最も優先度が高いメレー対象のみを残す
	for (const auto& candidate : m_melee_candidate)
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
		const auto candidate_obj = ObjManager::GetInstance()->GetObj<ObjBase>(distance.front().target_obj_handle);
		const auto melee_target = std::dynamic_pointer_cast<IMeleeHittable>(candidate_obj);
		m_player->AddMeleeTarget(melee_target);
		return;
	}

	// 角度でソート
	std::ranges::sort(angle, {}, &MeleeCandidateData::camera_diff_angle);
	const auto candidate_obj = ObjManager::GetInstance()->GetObj<ObjBase>(angle.front().target_obj_handle);
	const auto melee_target = std::dynamic_pointer_cast<IMeleeHittable>(candidate_obj);
	m_player->AddMeleeTarget(melee_target);
}

void MeleeTargetSearcher::SearchTopPriorityDownedChara()
{
	if (m_visible_downed_chara.empty() || m_player->GetMeleeTarget()) { return; }

	std::vector<MeleeCandidateData> distance;
	std::vector<MeleeCandidateData> angle;

	// MEMO : 画面中央に近い位置にいる場合は、カメラに距離が近い順にソート
	//		  そうでない場合は、カメラのforwardに近い順でソート
	//		  最終的に最も優先度が高いメレー対象のみを残す
	for (const auto& candidate : m_visible_downed_chara)
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
		std::ranges::sort(distance, {}, &MeleeCandidateData::distance_to_camera);
		const auto candidate_obj = ObjManager::GetInstance()->GetObj<ObjBase>(distance.front().target_obj_handle);
		const auto downed_chara = std::dynamic_pointer_cast<IMeleeHittable>(candidate_obj);
		m_player->AddTopPriorityDownedChara(downed_chara);
		return;
	}

	// 角度でソート
	std::ranges::sort(angle, {}, &MeleeCandidateData::camera_diff_angle);
	const auto candidate_obj = ObjManager::GetInstance()->GetObj<ObjBase>(angle.front().target_obj_handle);
	const auto downed_chara = std::dynamic_pointer_cast<IMeleeHittable>(candidate_obj);
	m_player->AddTopPriorityDownedChara(downed_chara);
}
