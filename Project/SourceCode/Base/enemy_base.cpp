#include "enemy_base.hpp"

EnemyBase::EnemyBase(const std::string& name, const MassKind mass_level_kind) :
	CharacterBase				(name, ObjTag.ENEMY, mass_level_kind),
	m_attack_interval_time		(0.0f),
	m_attack_interval_timer		(0.0f),
	m_can_action				(true),
	m_is_stop_action_forcibly	(false),
	m_is_target_in_sight		(false),
	m_enemy_handle				(HandleCreator::GetInstance()->CreateHandle(HandleCreator::Kind::kEnemy))
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

void EnemyBase::OnRespawn(const VECTOR& pos, const VECTOR& look_dir)
{
	// TODO : Init処理を入れる
	m_transform->SetPos(CoordinateKind::kWorld, pos);

	m_look_dir.at(TimeKind::kNext) = m_look_dir.at(TimeKind::kCurrent) = v3d::GetNormalizedV(look_dir);
	ApplyLookDirToRot(m_look_dir.at(TimeKind::kCurrent));
}

void EnemyBase::OnAllowAction()
{
	m_is_stop_action_forcibly = false;

	// 残りの攻撃インターバル時間が短すぎる場合は本来の時間の1/2の時間を与える
	if (m_attack_interval_timer < m_attack_interval_time * 0.25f)
	{
		m_attack_interval_timer = m_attack_interval_time * 0.5f;
	}
}
