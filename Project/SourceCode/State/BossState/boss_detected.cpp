#include "boss_detected.hpp"

boss_state::Detected::Detected() :
	ActionStateBase		(static_cast<int>(boss_state::ActionStateKind::kDetected)),
	m_is_stop_all_state	(false)
{

}

boss_state::Detected::~Detected()
{

}

void boss_state::Detected::Update(std::shared_ptr<Boss>& obj)
{
	obj->Detected();
}

void boss_state::Detected::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Detected::Enter(std::shared_ptr<Boss>& obj)
{
	const auto pos = obj->GetTransform()->GetPos(CoordinateKind::kWorld);

	const OnTargetDetectedEvent event{ obj->GetEnemyID(), obj->GetDetecteNotifyDistance(), pos };
	EventSystem::GetInstance()->Publish(event);
}

void boss_state::Detected::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::Detected::ChangeState(std::shared_ptr<Boss>& obj)
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
	// ノックバック
	if (state_controller->TryKnockback(obj))
	{
		return state_controller->GetState<Knockback, Boss>();
	}

	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
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
		// ダッシュ
		if (state_controller->TryRun(obj))
		{
			return state_controller->GetState<Run, Boss>();
		}

		// NULL
		return state_controller->GetState<ActionNull, Boss>();
	}

	return nullptr;
}
