#include "stealth_kill_target_searcher.hpp"

StealthKillTargetSearcher::StealthKillTargetSearcher(std::shared_ptr<Player>& player) : 
	m_player(player)
{
	// イベントの登録
	EventSystem::GetInstance()->Subscribe<OnNearEnemySpottedEvent>	(this, &StealthKillTargetSearcher::AddStealthKillCandidate);
}

StealthKillTargetSearcher::~StealthKillTargetSearcher()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<OnNearEnemySpottedEvent>(this, &StealthKillTargetSearcher::AddStealthKillCandidate);
}

void StealthKillTargetSearcher::Update()
{
	m_player->RemoveStealthKillTarget();

	SearchTarget();

	m_stealth_kill_candidate.clear();
}

void StealthKillTargetSearcher::AddStealthKillCandidate(const OnNearEnemySpottedEvent& event)
{
	m_stealth_kill_candidate.emplace_back(event.target_obj_handle, event.camera_diff_angle, event.distance_to_camera);
}

void StealthKillTargetSearcher::SearchTarget()
{
	if (m_stealth_kill_candidate.empty() || !m_player->CanSearchStealthKillTarget()) { return; }

	// ステルスキル対象とならない候補者を除外
	for (auto itr = m_stealth_kill_candidate.begin(); itr != m_stealth_kill_candidate.end(); )
	{
		// 候補者をインターフェイスに変更
		const auto candidate_obj	= ObjAccessor::GetInstance()->GetObj<ObjBase>(itr->target_obj_handle);
		const auto stealth_killable = std::dynamic_pointer_cast<IStealthKillable>(candidate_obj);

		// ステルスキルが許可されていない場合は除外
		if (!stealth_killable->IsAllowStealthKill())
		{
			itr = m_stealth_kill_candidate.erase(itr);
			continue;
		}

		// メレー対象かつ背後にいる場合は残す
		if (std::dynamic_pointer_cast<const ObjBase>(m_player->GetMeleeTarget()) == candidate_obj)
		{
			const auto candidate_transform	= candidate_obj->GetTransform();
			const auto target_forward		= candidate_transform->GetForward(CoordinateKind::kWorld);
			auto dir = m_player->GetTransform()->GetPos(CoordinateKind::kWorld) - candidate_transform->GetPos(CoordinateKind::kWorld);
			dir.y = 0.0f;
			dir = v3d::GetNormalizedV(dir);

			if (math::GetAngleBetweenTwoVectors(target_forward, dir) >= 135.0f * math::kDegToRad)
			{
				++itr;
				continue;
			}
		}

		// 見つかってない場合は残す
		if (!stealth_killable->IsStealthKillerInSight())
		{
			++itr;
			continue;
		}

		// 除外
		itr = m_stealth_kill_candidate.erase(itr);
	}

	if (m_stealth_kill_candidate.empty()) { return; }

	// 残った候補者をソートしてステルスキル対象を決定
	std::vector<StealthKillCandidateData> distance;
	std::vector<StealthKillCandidateData> angle;

	// MEMO : 画面中央に近い位置にいる場合は、カメラに距離が近い順にソート
	//		  そうでない場合は、カメラのforwardに近い順でソート
	//		  最終的に最も優先度が高いメレー対象のみを残す
	for (const auto& candidate : m_stealth_kill_candidate)
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
		std::ranges::sort(distance, {}, &StealthKillCandidateData::distance_to_camera);
		const auto candidate_obj = ObjAccessor::GetInstance()->GetObj<ObjBase>(distance.front().target_obj_handle);
		const auto stealth_killable = std::dynamic_pointer_cast<IStealthKillable>(candidate_obj);
		m_player->AddStealthKillTarget(stealth_killable);
		return;
	}

	// 角度でソート
	std::ranges::sort(angle, {}, &StealthKillCandidateData::camera_diff_angle);
	const auto candidate_obj = ObjAccessor::GetInstance()->GetObj<ObjBase>(angle.front().target_obj_handle);
	const auto stealth_killable = std::dynamic_pointer_cast<IStealthKillable>(candidate_obj);
	m_player->AddStealthKillTarget(stealth_killable);
}
