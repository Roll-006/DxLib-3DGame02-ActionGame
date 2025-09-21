#include "enemy_base.hpp"

EnemyBase::EnemyBase(const std::string& name, const MassKind mass_level_kind) :
	CharacterBase			(name, ObjTag.ENEMY, mass_level_kind),
	m_attack_interval_time	(0.0f),
	m_attack_interval_timer	(0.0f),
	m_can_action			(true),
	m_is_target_in_sight	(false)
{

}

void EnemyBase::SetAttackIntervalTime()
{
	m_attack_interval_timer = m_attack_interval_time;
}

void EnemyBase::CalcAttackIntervalTime()
{
	math::Decrease(m_attack_interval_timer, GetDeltaTime(), 0.0f);
}
