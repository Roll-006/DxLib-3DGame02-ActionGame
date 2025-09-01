#include "player_action_null.hpp"

player_state::ActionNull::ActionNull() : 
	ActionStateBase(static_cast<int>(player_state::ActionStateKind::kActionNull))
{

}

player_state::ActionNull::~ActionNull()
{

}

void player_state::ActionNull::Update(std::shared_ptr<Player> obj)
{
	const auto state_controller = obj->GetStateController();
	if (state_controller->GetMoveState(TimeKind::kCurrent)->GetStateKind() == static_cast<int>(player_state::MoveStateKind::kMove))
	{
		obj->DirOfCameraForward();
	}
}

void player_state::ActionNull::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::ActionNull::Enter(std::shared_ptr<Player> obj)
{

}

void player_state::ActionNull::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::ActionNull::ChangeState(std::shared_ptr<Player> obj)
{
	const auto state_controller = obj->GetStateController();

	// ƒ_ƒbƒVƒ…
	if (state_controller->TryRun())
	{
		return state_controller->GetState<Run, Player>();
	}
	// ‚µ‚á‚ª‚Þ
	if (CommandHandler::GetInstance()->IsExecute(CommandKind::kCrouch, TimeKind::kCurrent))
	{
		return state_controller->GetState<Crouch, Player>();
	}

	return nullptr;
}
