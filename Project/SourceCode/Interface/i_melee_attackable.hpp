#pragma once
#include "i_melee_hittable.hpp"

#include "../Event/on_downed_far_enemy_spotted_event.hpp"
#include "../Event/on_downed_near_enemy_spotted_event.hpp"

#include "../Data/melee_candidate_data.hpp"
#include "../Base/character_base.hpp"

/// @brief メレー攻撃を行う側
class IMeleeAttackable abstract
{
public:
	virtual ~IMeleeAttackable() = default;

	virtual void UpdateMelee() abstract;

	/// @brief 対象の前方で行うメレーを実行する
	virtual void SetupFrontMelee		(const VECTOR& target_pos, const VECTOR& target_dir) abstract;
	/// @brief 対象の後方で行うメレーを実行する
	virtual void SetupBackMelee			(const VECTOR& target_pos, const VECTOR& target_dir) abstract;
	/// @brief 対象に対してどの位置からでも行えるメレーを実行する
	virtual void SetupVersatilityMelee	(const VECTOR& target_pos) abstract;

	virtual void AttackFrontMelee		(CharacterBase* target) abstract;
	virtual void AttackBackMelee		(CharacterBase* target) abstract;
	virtual void AttackVersatilityMelee	(CharacterBase* target) abstract;

	/// @brief メレー候補をイベントにより追加する
	virtual void AddVisibleDownedCharacter	(const OnDownedFarEnemySpottedEvent&  event) abstract;
	virtual void AddMeleeCandidate			(const OnDownedNearEnemySpottedEvent& event) abstract;

	virtual void AddMeleeTarget(const int target_obj_handle) abstract;
	virtual void AddTopPriorityVisibleDownedCharacter(const int target_obj_handle) abstract;
	
	virtual [[nodiscard]] std::vector<MeleeCandidateData>	GetVisibleDownedCharacter()				const abstract;
	virtual [[nodiscard]] std::vector<MeleeCandidateData>	GetMeleeCandidate()						const abstract;
	virtual [[nodiscard]] std::shared_ptr<IMeleeHittable>&	GetTopPriorityVisibleDownedCharacter()		  abstract;
	virtual [[nodiscard]] std::shared_ptr<IMeleeHittable>&	GetMeleeTarget()							  abstract;
	
protected:
	virtual void RemoveVisibleDownedCharacter()				abstract;
	virtual void RemoveTopPriorityVisibleDownedCharacter()	abstract;
	virtual void RemoveMeleeCandidate()						abstract;
	virtual void RemoveMeleeTarget()						abstract;

	/// @brief 最優先の見えているダウン中のキャラクターを決定する
	virtual void DecisionTopPriorityVisibleDownedCharacter() abstract;

	/// @brief メレー対象を決定する
	virtual void DecisionMeleeTarget() abstract;
};
