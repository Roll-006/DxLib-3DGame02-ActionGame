#pragma once
#include "../Object/player.hpp"
#include "../Data/stealth_kill_candidate_data.hpp"

class StealthKillTargetSearcher final
{
public:
	StealthKillTargetSearcher(std::shared_ptr<Player>& player);
	~StealthKillTargetSearcher();

	void Update();

private:
	#pragma region Event
	void AddStealthKillCandidate(const OnNearEnemySpottedEvent& event);
	#pragma endregion


	void SearchTarget();

private:
	static constexpr float kPrioritySwitchAngle = 30.0f;

	std::shared_ptr<Player>&				m_player;
	std::vector<StealthKillCandidateData>	m_stealth_kill_candidate;
};
