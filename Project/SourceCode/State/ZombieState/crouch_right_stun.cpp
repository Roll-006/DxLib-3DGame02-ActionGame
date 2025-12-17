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

	obj->DisallowDecreaseKnockBackGauge();
	obj->AllowCalcLookDir();
}

void zombie_state::CrouchRightStun::LateUpdate(std::shared_ptr<Zombie>& obj)
{
	obj->OnRightCrouchIK();
}

void zombie_state::CrouchRightStun::Enter(std::shared_ptr<Zombie>& obj)
{
	m_stun_timer = 0.0f;

	obj->GetHealth(HealthPartKind::kRightLeg)->IncreaseCurrentMax();

	const auto model_handle = obj->GetModeler()->GetModelHandle();
	auto	   head_m		= MV1GetFrameLocalWorldMatrix(model_handle, obj->GetHumanoidFrame()->GetHeadIndex(model_handle));
	const auto head_pos		= MGetTranslateElem(head_m);
	EventSystem::GetInstance()->Publish(StunEvent(head_pos));

	// 撃たれた反動で回転させる
	obj->OnRotate(30.0f * math::kDegToRad, RotDirKind::kRight);

	const auto humanoid = std::dynamic_pointer_cast<IHumanoid>(obj);
	obj->GetHumanoidFootIKSolver()->CreateRightLegRay (obj.get(), humanoid);
	obj->GetHumanoidArmIKSolver ()->CreateRightHandRay(obj.get(), humanoid);
}

void zombie_state::CrouchRightStun::Exit(std::shared_ptr<Zombie>& obj)
{
	const auto model_handle = obj->GetModeler()->GetModelHandle();
	auto	   head_m		= MV1GetFrameLocalWorldMatrix(model_handle, obj->GetHumanoidFrame()->GetHeadIndex(model_handle));
	const auto head_pos		= MGetTranslateElem(head_m);
	EventSystem::GetInstance()->Publish(ExitStunEvent(head_pos));

	const auto humanoid = std::dynamic_pointer_cast<IHumanoid>(obj);
	obj->GetHumanoidFootIKSolver()->DeleteRightLegRay (obj.get());
	obj->GetHumanoidArmIKSolver ()->DeleteRightHandRay(obj.get());
}

std::shared_ptr<IState<Zombie>> zombie_state::CrouchRightStun::ChangeState(std::shared_ptr<Zombie>& obj)
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
