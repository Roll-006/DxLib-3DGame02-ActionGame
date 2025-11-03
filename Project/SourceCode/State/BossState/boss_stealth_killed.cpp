#include "boss_stealth_killed.hpp"

boss_state::StealthKilled::StealthKilled() :
	ActionStateBase		(static_cast<int>(boss_state::ActionStateKind::kStealthKilled)),
	m_is_stop_all_state	(true)
{

}

boss_state::StealthKilled::~StealthKilled()
{

}

void boss_state::StealthKilled::Update(std::shared_ptr<Boss>& obj)
{
	obj->DisallowStealthKill();
}

void boss_state::StealthKilled::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::StealthKilled::Enter(std::shared_ptr<Boss>& obj)
{
	// ’Í‚Ü‚ê‚½‚±‚Æ‚ð‰‰oƒJƒƒ‰‚É’Ê’m
	const OnStealthKillEvent event{ obj->GetEnemyID(), obj->GetModeler() };
	EventSystem::GetInstance()->Publish(event);
}

void boss_state::StealthKilled::Exit(std::shared_ptr<Boss>& obj)
{
	obj->ExitStealthKilled();
}

std::shared_ptr<IState<Boss>> boss_state::StealthKilled::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		// Ž€–S
		if (state_controller->TryDead(obj))
		{
			return state_controller->GetState<Dead, Boss>();
		}

		// —§‚¿ã‚ª‚é
		return state_controller->GetState<StandUp, Boss>();
	}

	return nullptr;
}
