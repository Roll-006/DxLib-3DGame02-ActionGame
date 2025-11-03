#include "boss_run.hpp"

boss_state::Run::Run() :
	ActionStateBase		(static_cast<int>(boss_state::ActionStateKind::kRun)),
	m_is_stop_all_state	(false)
{

}

boss_state::Run::~Run()
{

}

void boss_state::Run::Update(std::shared_ptr<Boss>& obj)
{
	obj->CalcMoveSpeedRun();

	if (obj->CanAction())
	{
		obj->CalcAttackIntervalTime();
	}
}

void boss_state::Run::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Run::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Run::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::Run::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// 強制NULL
	if (state_controller->TryActionNullForcibly(obj))
	{
		return state_controller->GetState<ActionNull, Boss>();
	}
	// ステルスキルされた
	if (state_controller->TryStealthKilled(obj))
	{
		return state_controller->GetState<StealthKilled, Boss>();
	}
	// 発見
	if (state_controller->TryDetected(obj))
	{
		return state_controller->GetState<Detected, Boss>();
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
	// 左足ダウン
	if (state_controller->TryLeftCrouchStun(obj))
	{
		return state_controller->GetState<CrouchLeftStun, Boss>();
	}
	// 右足ダウン
	if (state_controller->TryRightCrouchStun(obj))
	{
		return state_controller->GetState<CrouchRightStun, Boss>();
	}
	// 立ちダウン
	if (state_controller->TryStandStun(obj))
	{
		return state_controller->GetState<StandStun, Boss>();
	}
	//// ダッシュ掴み
	//if (state_controller->TryGrabRun())
	//{
	//	return state_controller->GetState<GrabRun, Boss>();
	//}
	// NULL
	if (state_controller->TryWalk(obj))
	{
		return state_controller->GetState<ActionNull, Boss>();
	}

	return nullptr;
}
