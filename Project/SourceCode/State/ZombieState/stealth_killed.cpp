#include "stealth_killed.hpp"

zombie_state::StealthKilled::StealthKilled() :
	ActionStateBase		(static_cast<int>(zombie_state::ActionStateKind::kStealthKilled)),
	m_is_stop_all_state	(true)
{

}

zombie_state::StealthKilled::~StealthKilled()
{

}

void zombie_state::StealthKilled::Update(std::shared_ptr<Zombie>& obj)
{
	obj->DisallowStealthKill();
}

void zombie_state::StealthKilled::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::StealthKilled::Enter(std::shared_ptr<Zombie>& obj)
{
	// ’Í‚Ü‚ê‚½‚±‚Æ‚ð‰‰oƒJƒƒ‰‚É’Ê’m
	const OnStealthKillEvent event{ obj->GetEnemyID(), obj->GetModeler() };
	EventSystem::GetInstance()->Publish(event);
}

void zombie_state::StealthKilled::Exit(std::shared_ptr<Zombie>& obj)
{
	obj->ExitStealthKilled();
}

std::shared_ptr<IState<Zombie>> zombie_state::StealthKilled::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		// Ž€–S
		if (state_controller->TryDead(obj))
		{
			return state_controller->GetState<Dead, Zombie>();
		}

		// —§‚¿ã‚ª‚é
		return state_controller->GetState<StandUp, Zombie>();
	}

	return nullptr;
}
