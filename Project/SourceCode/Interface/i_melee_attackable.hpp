#pragma once
#include "i_melee_hittable.hpp"

#include "../Event/on_downed_far_enemy_spotted_event.hpp"
#include "../Event/on_downed_near_enemy_spotted_event.hpp"

#include "../Data/spotted_obj_data.hpp"
#include "../Base/character_base.hpp"

/// @brief メレー攻撃を行う側
class IMeleeAttackable abstract
{
public:
	virtual ~IMeleeAttackable() = default;

	virtual void UpdateMelee() abstract;

	virtual void StopSearchMeleeTarget() abstract;

	/// @brief 対象の前方で行うメレーを実行する
	virtual void SetupFrontMelee()			abstract;
	/// @brief 対象の後方で行うメレーを実行する
	virtual void SetupBackMelee()			abstract;
	/// @brief 対象に対してどの位置からでも行えるメレーを実行する
	virtual void SetupVersatilityMelee()	abstract;

	virtual void AttackFrontMelee		(CharacterBase* target) abstract;
	virtual void AttackBackMelee		(CharacterBase* target) abstract;
	virtual void AttackVersatilityMelee	(CharacterBase* target) abstract;

	virtual void AddTopPriorityDownedChara	(const std::shared_ptr<IMeleeHittable>& downed_character)	abstract;
	virtual void AddMeleeTarget				(const std::shared_ptr<IMeleeHittable>& melee_target)		abstract;
	virtual void RemoveTopPriorityDownedChara()	abstract;
	virtual void RemoveMeleeTarget()			abstract;
	
	[[nodiscard]] virtual std::shared_ptr<IMeleeHittable>&	GetTopPriorityDownedChara()		abstract;
	[[nodiscard]] virtual std::shared_ptr<IMeleeHittable>&	GetMeleeTarget()				abstract;
	[[nodiscard]] virtual const bool						CanSearchMeleeTarget() const	abstract;
};
