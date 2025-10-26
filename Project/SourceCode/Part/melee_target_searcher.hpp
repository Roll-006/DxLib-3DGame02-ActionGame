#pragma once
#include "../Object/player.hpp"
#include "../Calculation/math.hpp"

class MeleeTargetSearcher final
{
public:
	MeleeTargetSearcher(std::shared_ptr<Player>& player);
	~MeleeTargetSearcher();

	void Update();

private:
	#pragma region Event
	void AddVisibleDownedCharacter	(const OnDownedFarEnemySpottedEvent&  event);
	void AddMeleeCandidate			(const OnDownedNearEnemySpottedEvent& event);
	#pragma endregion


	void SearchMeleeTarget();
	void SearchTopPriorityDownedChara();

private:
	static constexpr float kPrioritySwitchAngle = 30.0f;

	std::shared_ptr<Player>&		m_player;
	std::vector<MeleeCandidateData> m_visible_downed_chara;		// ダウンしている見えている敵
	std::vector<MeleeCandidateData> m_melee_candidate;			// メレーの候補者リスト
};
