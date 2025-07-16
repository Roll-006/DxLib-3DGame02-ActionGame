#include "crouch.hpp"

player_state::Crouch::Crouch() :
	ActionStateBase(static_cast<int>(player_state::ActionStateKind::kCrouch))
{

}

player_state::Crouch::~Crouch()
{

}

void player_state::Crouch::Update(Player* obj)
{

}

void player_state::Crouch::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::Crouch::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();

	if (state_controller->TryRun())
	{
		return state_controller->GetState<Run, Player>();
	}

	return nullptr;
}
