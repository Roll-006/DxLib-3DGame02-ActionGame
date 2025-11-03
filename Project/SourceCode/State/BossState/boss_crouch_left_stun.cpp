#include "boss_crouch_left_stun.hpp"

boss_state::CrouchLeftStun::CrouchLeftStun() :
	ActionStateBase		(static_cast<int>(boss_state::ActionStateKind::kCrouchLeftStun)),
	m_stun_timer		(0.0f),
	m_is_stop_all_state	(false)
{

}

boss_state::CrouchLeftStun::~CrouchLeftStun()
{

}

void boss_state::CrouchLeftStun::Update(std::shared_ptr<Boss>& obj)
{
	if (obj->CanAction())
	{
		m_stun_timer += obj->GetDeltaTime();
	}

	//obj->CalcAttackIntervalTime();
}

void boss_state::CrouchLeftStun::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::CrouchLeftStun::Enter(std::shared_ptr<Boss>& obj)
{
	m_stun_timer = 0.0f;

	obj->GetHealth(HealthPartKind::kLeftLeg)->IncreaseCurrentMax();
}

void boss_state::CrouchLeftStun::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::CrouchLeftStun::ChangeState(std::shared_ptr<Boss>& obj)
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
