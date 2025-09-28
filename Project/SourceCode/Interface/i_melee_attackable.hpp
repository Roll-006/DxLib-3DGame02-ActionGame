#pragma once
#include "../Event/on_downed_enemy_spotted_event.hpp"
#include "../Data/melee_candidate_data.hpp"

/// @brief メレー攻撃を行う側
class IMeleeAttackable abstract
{
public:
	virtual ~IMeleeAttackable() = default;

	/// @brief メレー候補をイベントにより追加する
	virtual void AddMeleeCandidate(const OnDownedEnemySpottedEvent& event) abstract;

	virtual [[nodiscard]] std::vector<MeleeCandidateData> GetMeleeCandidate() const abstract;
	
protected:
	/// @brief メレー候補の登録を解除する
	virtual void RemoveMeleeCandidate() abstract;

	/// @brief メレー対象を決定する
	virtual void DecisionMeleeTarget()  abstract;
};
