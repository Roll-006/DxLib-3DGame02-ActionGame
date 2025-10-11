#pragma once
#include "i_melee_hittable.hpp"

#include "../Event/on_downed_enemy_spotted_event.hpp"
#include "../Event/on_melee_target_lost_event.hpp"

#include "../Data/melee_candidate_data.hpp"
#include "../Base/character_base.hpp"

/// @brief メレー攻撃を行う側
class IMeleeAttackable abstract
{
public:
	virtual ~IMeleeAttackable() = default;

	virtual void UpdateMelee() abstract;

	/// @brief 対象の前方で行うメレーを実行する
	virtual void AttackFrontMelee		(const VECTOR& target_pos, const VECTOR& target_dir) abstract;
	/// @brief 対象の後方で行うメレーを実行する
	virtual void AttackBackMelee		(const VECTOR& target_pos, const VECTOR& target_dir) abstract;
	/// @brief 対象に対してどの位置からでも行えるメレーを実行する
	virtual void AttackVersatilityMelee	(const VECTOR& target_pos) abstract;

	virtual void OnCollideFrontMelee		(CharacterBase* target) abstract;
	virtual void OnCollideBackMelee			(CharacterBase* target) abstract;
	virtual void OnCollideVersatilityMelee	(CharacterBase* target) abstract;

	/// @brief メレー候補をイベントにより追加する
	virtual void AddMeleeCandidate(const OnDownedEnemySpottedEvent& event)	abstract;
	virtual void RemoveMeleeTarget(const OnMeleeTargetLostEvent& event)		abstract;

	virtual void AddMeleeTarget(const int target_obj_handle) abstract;
	
	virtual [[nodiscard]] std::vector<MeleeCandidateData>	GetMeleeCandidate()	const abstract;
	virtual [[nodiscard]] std::shared_ptr<IMeleeHittable>&	GetMeleeTarget()		  abstract;
	
protected:
	virtual void RemoveMeleeCandidate() abstract;
	virtual void RemoveMeleeTarget()	abstract;

	/// @brief メレー対象を決定する
	virtual void DecisionMeleeTarget()  abstract;
};
