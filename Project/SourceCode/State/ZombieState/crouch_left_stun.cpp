#include "crouch_left_stun.hpp"

zombie_state::CrouchLeftStun::CrouchLeftStun() :
	ActionStateBase		(static_cast<int>(zombie_state::ActionStateKind::kCrouchLeftStun)),
	m_stun_timer		(0.0f),
	m_is_stop_all_state	(false)
{

}

zombie_state::CrouchLeftStun::~CrouchLeftStun()
{

}

void zombie_state::CrouchLeftStun::Update(std::shared_ptr<Zombie>& obj)
{
	if (obj->CanAction())
	{
		m_stun_timer += obj->GetDeltaTime();
	}

	obj->DisallowDecreaseKnockBackGauge();
	obj->AllowCalcLookDir();
}

void zombie_state::CrouchLeftStun::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::CrouchLeftStun::Enter(std::shared_ptr<Zombie>& obj)
{
	m_stun_timer = 0.0f;

	obj->GetHealth(HealthPartKind::kLeftLeg)->IncreaseCurrentMax();

	const auto model_handle = obj->GetModeler()->GetModelHandle();
	auto	   head_m		= MV1GetFrameLocalWorldMatrix(model_handle, obj->GetHumanoidFrame()->GetHeadIndex(model_handle));
	const auto head_pos		= MGetTranslateElem(head_m);
	EventSystem::GetInstance()->Publish(StunEvent(head_pos));

	// 撃たれた反動で回転させる
	obj->OnRotate(30.0f * math::kDegToRad, RotDirKind::kLeft);
}

void zombie_state::CrouchLeftStun::Exit(std::shared_ptr<Zombie>& obj)
{
	const auto model_handle = obj->GetModeler()->GetModelHandle();
	auto	   head_m		= MV1GetFrameLocalWorldMatrix(model_handle, obj->GetHumanoidFrame()->GetHeadIndex(model_handle));
	const auto head_pos		= MGetTranslateElem(head_m);
	EventSystem::GetInstance()->Publish(ExitStunEvent(head_pos));
}

std::shared_ptr<IState<Zombie>> zombie_state::CrouchLeftStun::ChangeState(std::shared_ptr<Zombie>& obj)
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
	// NULL
	if (m_stun_timer > kStunTime)
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}

	return nullptr;
}
