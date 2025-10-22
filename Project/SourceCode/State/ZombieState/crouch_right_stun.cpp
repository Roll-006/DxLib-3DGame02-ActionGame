#include "crouch_right_stun.hpp"

zombie_state::CrouchRightStun::CrouchRightStun() :
	ActionStateBase		(static_cast<int>(zombie_state::ActionStateKind::kCrouchRightStun)),
	m_stun_timer		(0.0f),
	m_is_stop_all_state	(false)
{

}

zombie_state::CrouchRightStun::~CrouchRightStun()
{

}

void zombie_state::CrouchRightStun::Update(std::shared_ptr<Zombie>& obj)
{
	if (obj->CanAction())
	{
		m_stun_timer += obj->GetDeltaTime();
	}

	//obj->CalcAttackIntervalTime();
}

void zombie_state::CrouchRightStun::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::CrouchRightStun::Enter(std::shared_ptr<Zombie>& obj)
{
	m_stun_timer = 0.0f;

	obj->GetHealth(HealthPartKind::kRightLeg)->IncreaseCurrentMax();
}

void zombie_state::CrouchRightStun::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::CrouchRightStun::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

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
	// NULL
	if (m_stun_timer > kStunTime)
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}
	         
	return nullptr;
}
