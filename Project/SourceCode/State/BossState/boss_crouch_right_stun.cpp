#include "boss_crouch_right_stun.hpp"

boss_state::CrouchRightStun::CrouchRightStun() :
	ActionStateBase		(static_cast<int>(boss_state::ActionStateKind::kCrouchRightStun)),
	m_stun_timer		(0.0f),
	m_is_stop_all_state	(false)
{

}

boss_state::CrouchRightStun::~CrouchRightStun()
{

}

void boss_state::CrouchRightStun::Update(std::shared_ptr<Boss>& obj)
{
	if (obj->CanAction())
	{
		m_stun_timer += obj->GetDeltaTime();
	}

	//obj->CalcAttackIntervalTime();
}

void boss_state::CrouchRightStun::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::CrouchRightStun::Enter(std::shared_ptr<Boss>& obj)
{
	m_stun_timer = 0.0f;

	obj->GetHealth(HealthPartKind::kRightLeg)->IncreaseCurrentMax();
}

void boss_state::CrouchRightStun::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::CrouchRightStun::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// ステルスキルされた
	if (state_controller->TryStealthKilled(obj))
	{
		return state_controller->GetState<StealthKilled, Boss>();
	}
	// ノックバック
	if (state_controller->TryKnockback(obj))
	{
		return state_controller->GetState<Knockback, Boss>();
	}
	// 死亡
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Boss>();
	}
	// NULL
	if (m_stun_timer > kStunTime)
	{
		return state_controller->GetState<ActionNull, Boss>();
	}
	         
	return nullptr;
}
