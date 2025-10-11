#include "stand_stun.hpp"

zombie_state::StandStun::StandStun() :
	ActionStateBase		(static_cast<int>(zombie_state::ActionStateKind::kStandStun)),
	m_stun_timer		(0.0f),
	m_is_stop_all_state	(false)
{

}

zombie_state::StandStun::~StandStun()
{

}

void zombie_state::StandStun::Update(std::shared_ptr<Zombie>& obj)
{
	if (obj->CanAction())
	{
		m_stun_timer += obj->GetDeltaTime();
	}

	//obj->CalcAttackIntervalTime();
}

void zombie_state::StandStun::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::StandStun::Enter(std::shared_ptr<Zombie>& obj)
{
	m_stun_timer = 0.0f;

	obj->GetHealth(HealthPartKind::kHead)->IncreaseMax();
}

void zombie_state::StandStun::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::StandStun::ChangeState(std::shared_ptr<Zombie>& obj)
{
	const auto state_controller = obj->GetStateController();

	// ノックバック
	if (state_controller->TryKnockback(obj))
	{
		return state_controller->GetState<Knockback, Zombie>();
	}
	// 死亡
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Zombie>();
	}
	// 左足ダウン
	if (state_controller->TryLeftCrouchStun(obj))
	{
		return state_controller->GetState<CrouchLeftStun, Zombie>();
	}
	// 右足ダウン
	if (state_controller->TryRightCrouchStun(obj))
	{
		return state_controller->GetState<CrouchRightStun, Zombie>();
	}
	// NULL
	if (m_stun_timer > kStunTime)
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}

	return nullptr;
}
