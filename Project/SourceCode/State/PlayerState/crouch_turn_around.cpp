#include "crouch_turn_around.hpp"

player_state::CrouchTurnAround::CrouchTurnAround() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kCrouchTurnAround)),
	m_is_stop_all_state	(false)
{

}

player_state::CrouchTurnAround::~CrouchTurnAround()
{

}

void player_state::CrouchTurnAround::Update(std::shared_ptr<Player>& obj)
{

}

void player_state::CrouchTurnAround::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::CrouchTurnAround::Enter(std::shared_ptr<Player>& obj)
{

}

void player_state::CrouchTurnAround::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::CrouchTurnAround::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// Ž€–S
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Player>();
	}
	// ƒƒŒ[(³–ÊR‚è)
	if (state_controller->TryFrontKick(obj))
	{
		return state_controller->GetState<FrontKick, Player>();
	}
	// ƒƒŒ[(‰ñ‚µR‚è)
	if (state_controller->TryRoundhouseKick(obj))
	{
		return state_controller->GetState<RoundhouseKick, Player>();
	}
	// ƒXƒeƒ‹ƒXƒLƒ‹
	if (state_controller->TryStealthKill(obj))
	{
		return state_controller->GetState<StealthKill, Player>();
	}
	// •ß‚Ü‚ê‚é
	if (state_controller->TryGrabbed(obj))
	{
		return state_controller->GetState<Grabbed, Player>();
	}

	return nullptr;
}
