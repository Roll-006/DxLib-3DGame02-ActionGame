#include "zombie_run.hpp"

zombie_state::Run::Run() :
	ActionStateBase		(static_cast<int>(zombie_state::ActionStateKind::kRun)),
	m_is_stop_all_state	(false)
{

}

zombie_state::Run::~Run()
{

}

void zombie_state::Run::Update(std::shared_ptr<Zombie>& obj)
{
	obj->CalcMoveSpeedRun();

	if (obj->CanAction())
	{
		obj->CalcAttackIntervalTime();
	}
}

void zombie_state::Run::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Run::Enter(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Run::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Run::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// 強制NULL
	if (state_controller->TryActionNullForcibly(obj))
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}
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
	// 立ちダウン
	if (state_controller->TryStandStun(obj))
	{
		return state_controller->GetState<StandStun, Zombie>();
	}
	// ダッシュ掴み
	if (state_controller->TryGrabRun())
	{
		return state_controller->GetState<GrabRun, Zombie>();
	}
	// NULL
	if (state_controller->TryWalk(obj))
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}

	return nullptr;
}
