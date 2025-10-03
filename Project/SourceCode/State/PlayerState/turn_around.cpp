#include "turn_around.hpp"

player_state::TurnAround::TurnAround() :
	ActionStateBase	(static_cast<int>(player_state::ActionStateKind::kTurnAround)),
	m_is_all_stop	(false)
{

}

player_state::TurnAround::~TurnAround()
{

}

void player_state::TurnAround::Update(std::shared_ptr<Player>& obj)
{

}

void player_state::TurnAround::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::TurnAround::Enter(std::shared_ptr<Player>& obj)
{

}

void player_state::TurnAround::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::TurnAround::ChangeState(std::shared_ptr<Player>& obj)
{
	const auto state_controller = obj->GetStateController();

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
	// •ß‚Ü‚ê‚é
	if (state_controller->TryGrabbed(obj))
	{
		return state_controller->GetState<Grabbed, Player>();
	}

	return nullptr;
}
