#include "enemy_base.hpp"

EnemyBase::EnemyBase(const std::string& name) :
	CharacterBase					(name, ObjTag.ENEMY),
	m_patrol_route_giver			(nullptr),
	m_patrol_destination_pos		(v3d::GetZeroV()),
	attack_interval_time			(0.0f),
	m_attack_interval_timer			(0.0f),
	m_can_action					(true),
	m_is_disallow_action_forcibly	(false),
	m_disappear_down_speed			(0.0f),
	m_on_collided_vision_trigger	(false),
	m_has_obstacle_between_target	(false),
	m_is_detected_target			(false),
	m_is_prev_detected_target		(false),
	m_is_detection_shared			(false),
	enemy_id						(""),
	m_knock_back_gauge				(nullptr),
	m_can_decrease_knock_back_gauge	(true)
{

}

void EnemyBase::SetUpAttackIntervalTime()
{
	m_attack_interval_timer = attack_interval_time;
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
	m_patrol_route_giver->ChangeDestination(m_patrol_destination_pos, pos);

	// 追跡が終了した場合は移動させない
	if (m_patrol_route_giver->IsEnd()) { return; }

	const auto pos_y0				= VGet(pos.x, 0.0f, pos.z);
	const auto destination_pos_y0	= VGet(m_patrol_destination_pos.x, 0.0f, m_patrol_destination_pos.z);
	
	m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(destination_pos_y0 - pos_y0);
}

void EnemyBase::OnRotate(const float angle, const RotDirKind rot_dir_kind)
{
	const auto up			= m_transform->GetUp(CoordinateKind::kWorld);
	const auto result_angle = rot_dir_kind == RotDirKind::kLeft ? -angle : angle;
	m_look_dir.at(TimeKind::kNext) = math::GetRotatedPos(m_look_dir.at(TimeKind::kCurrent), quat::CreateQuaternion(up, result_angle));
}

void EnemyBase::OnDetected()
{
	m_is_detection_shared = true;
}

void EnemyBase::Disappear()
{
	m_is_calc_look_dir = false;

	m_move_dir.at(TimeKind::kNext) = m_move_dir.at(TimeKind::kCurrent) = VGet(0.0f, -1.0f, 0.0f);
	math::Increase(m_disappear_down_speed, 0.01f * GetDeltaTime(), 0.072f, false);

	m_move_speed = m_disappear_down_speed;
}

void EnemyBase::OnAllowAction()
{
	m_is_disallow_action_forcibly = false;

	// 残りの攻撃インターバル時間は分を切っている場合は最大値にする
	if (m_attack_interval_timer < attack_interval_time * 0.5f)
	{
		m_attack_interval_timer = attack_interval_time;
	}
}

void EnemyBase::JudgeTargetInSight()
{
	m_is_prev_detected_target = m_is_detected_target;

	// 自分自身が発見した、もしくは発見状態が共有された場合に発見したものとする
	const auto is_detected = m_on_collided_vision_trigger && !m_has_obstacle_between_target;
	if (is_detected || m_is_detection_shared)
	{
		m_is_detected_target = true;
	}

	m_has_obstacle_between_target = false;
}
