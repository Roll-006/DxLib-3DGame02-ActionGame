#include "stealth_kill.hpp"

player_state::StealthKill::StealthKill() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kStealthKill)),
	m_is_stop_all_state	(true)
{

}

player_state::StealthKill::~StealthKill()
{

}

void player_state::StealthKill::Update(std::shared_ptr<Player>& obj)
{
	obj->UpdateStealthKill();
}

void player_state::StealthKill::LateUpdate(std::shared_ptr<Player>& obj)
{
	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::StealthKill::Enter(std::shared_ptr<Player>& obj)
{
	obj->SetupStealthKill();
}

void player_state::StealthKill::Exit(std::shared_ptr<Player>& obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

std::shared_ptr<IState<Player>> player_state::StealthKill::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// NULL
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<ActionNull, Player>();
	}

	return nullptr;
}
