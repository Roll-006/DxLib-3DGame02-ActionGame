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
	obj->OnFootIK();
}

void zombie_state::StandStun::Enter(std::shared_ptr<Zombie>& obj)
{
	m_stun_timer = 0.0f;

	obj->GetHealth(HealthPartKind::kHead)->IncreaseCurrentMax();

	const auto model_handle = obj->GetModeler()->GetModelHandle();
	auto	   head_m		= MV1GetFrameLocalWorldMatrix(model_handle, obj->GetHumanoidFrame()->GetHeadIndex(model_handle));
	const auto head_pos		= MGetTranslateElem(head_m);
	EventSystem::GetInstance()->Publish(StunEvent(head_pos));
}

void zombie_state::StandStun::Exit(std::shared_ptr<Zombie>& obj)
{
	const auto model_handle = obj->GetModeler()->GetModelHandle();
	auto	   head_m		= MV1GetFrameLocalWorldMatrix(model_handle, obj->GetHumanoidFrame()->GetHeadIndex(model_handle));
	const auto head_pos		= MGetTranslateElem(head_m);
	EventSystem::GetInstance()->Publish(ExitStunEvent(head_pos));
}

std::shared_ptr<IState<Zombie>> zombie_state::StandStun::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// ステルスキルされた
	if (state_controller->TryStealthKilled(obj))
	{
		return state_controller->GetState<StealthKilled, Zombie>();
	}
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
