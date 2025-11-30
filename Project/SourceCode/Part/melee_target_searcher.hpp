#pragma once
#include "../Interface/i_melee_attackable.hpp"
#include "../Calculation/math.hpp"

class MeleeTargetSearcher final
{
public:
	MeleeTargetSearcher(const std::weak_ptr<IMeleeAttackable>& melee_attackable);
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

	std::weak_ptr<IMeleeAttackable>	m_melee_attackable;
	std::vector<SpottedObjData>		m_visible_downed_chara;		// ダウンしている見えている敵
	std::vector<SpottedObjData>		m_melee_candidates;			// メレーの候補者リスト
};
