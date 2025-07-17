#include "action_null.hpp"

player_state::ActionNull::ActionNull() : 
	ActionStateBase(static_cast<int>(player_state::ActionStateKind::kActionNull))
{

}

player_state::ActionNull::~ActionNull()
{

}

void player_state::ActionNull::Update(Player* obj)
{

}

void player_state::ActionNull::Enter(Player* obj)
{

}

void player_state::ActionNull::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::ActionNull::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();

	if (state_controller->TryRun())
	{
		return state_controller->GetState<Run, Player>();
	}
	if (state_controller->TryCrouch())
	{
		return state_controller->GetState<Crouch, Player>();
	}

	return nullptr;
}
