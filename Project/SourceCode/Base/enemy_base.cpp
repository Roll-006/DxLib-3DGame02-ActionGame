#include "enemy_base.hpp"

EnemyBase::EnemyBase(const std::string& name, const MassKind mass_level_kind) :
	CharacterBase					(name, ObjTag.ENEMY, mass_level_kind),
	m_patrol_route_giver			(nullptr),
	m_attack_interval_time			(0.0f),
	m_attack_interval_timer			(0.0f),
	m_can_action					(true),
	m_is_disallow_action_forcibly	(false),
	m_on_collided_vision_trigger	(false),
	m_has_obstacle_between_target	(false),
	m_is_lost_target				(true),
	m_enemy_handle					(HandleCreator::GetInstance()->CreateHandle(HandleCreator::Kind::kEnemy))
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

void EnemyBase::CreatePatrolPos(const PatrolRouteGiver::PatrolKind patrol_kind, const std::string& route_id)
{
	m_patrol_route_giver = std::make_shared<PatrolRouteGiver>(patrol_kind, route_id);
}

void EnemyBase::ChangePatrolDestination()
{
	if (!m_patrol_route_giver) { return; }

	const auto pos = m_transform->GetPos(CoordinateKind::kWorld);
	m_patrol_route_giver->ChangeDestination(m_destination_pos, pos);
}

void EnemyBase::OnAllowAction()
{
	m_is_disallow_action_forcibly = false;

	// 残りの攻撃インターバル時間が短すぎる場合は本来の時間の1/2の時間を与える
	if (m_attack_interval_timer < m_attack_interval_time * 0.25f)
	{
		m_attack_interval_timer = m_attack_interval_time * 0.5f;
	}
}

void EnemyBase::JudgeLostTarget()
{
	if (m_on_collided_vision_trigger && !m_has_obstacle_between_target)
	{
		m_is_lost_target = false;
	}
}
