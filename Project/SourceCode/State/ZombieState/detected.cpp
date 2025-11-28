#include "detected.hpp"

zombie_state::Detected::Detected() :
	ActionStateBase		(static_cast<int>(zombie_state::ActionStateKind::kDetected)),
	m_is_stop_all_state	(false)
{

}

zombie_state::Detected::~Detected()
{

}

void zombie_state::Detected::Update(std::shared_ptr<Zombie>& obj)
{
	obj->Detected();
}

void zombie_state::Detected::LateUpdate(std::shared_ptr<Zombie>& obj)
{
	obj->OnFootIK();
}

void zombie_state::Detected::Enter(std::shared_ptr<Zombie>& obj)
{
	const auto pos = obj->GetTransform()->GetPos(CoordinateKind::kWorld);

	const OnTargetDetectedEvent event{ obj->GetEnemyID(), obj->GetDetecteNotifyDistance(), pos };
	EventSystem::GetInstance()->Publish(event);
}

void zombie_state::Detected::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Detected::ChangeState(std::shared_ptr<Zombie>& obj)
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

	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
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
		// ダッシュ
		if (state_controller->TryRun(obj))
		{
			return state_controller->GetState<Run, Zombie>();
		}

		// NULL
		return state_controller->GetState<ActionNull, Zombie>();
	}

	return nullptr;
}
