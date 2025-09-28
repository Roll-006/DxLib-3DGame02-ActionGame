#include "crouch_left_stun.hpp"

zombie_state::CrouchLeftStun::CrouchLeftStun() :
	ActionStateBase	(static_cast<int>(zombie_state::ActionStateKind::kCrouchLeftStun)),
	m_stun_timer	(0.0f),
	m_is_all_stop	(false)
{

}

zombie_state::CrouchLeftStun::~CrouchLeftStun()
{

}

void zombie_state::CrouchLeftStun::Update(std::shared_ptr<Zombie> obj)
{
	if (obj->CanAction())
	{
		m_stun_timer += obj->GetDeltaTime();
	}

	//obj->CalcAttackIntervalTime();
}

void zombie_state::CrouchLeftStun::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::CrouchLeftStun::Enter(std::shared_ptr<Zombie> obj)
{
	m_stun_timer = 0.0f;

	obj->GetHealth(HealthPartKind::kLeftLeg)->RecoverMax();
}

void zombie_state::CrouchLeftStun::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::CrouchLeftStun::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	// Ž€–S
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
