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
	obj->CalcCrouchSpeed();
	obj->DirOfCameraForward();
}

void player_state::Crouch::Enter(Player* obj)
{

}

void player_state::Crouch::Exit(Player* obj)
{
	CommandHandler::GetInstance()->InitTriggerCount(CommandKind::kCrouch);
}

std::shared_ptr<IState<Player>> player_state::Crouch::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();

	if (!state_controller->TryCrouch())
	{
		return state_controller->GetState<ActionNull, Player>();
	}
	if (state_controller->TryRun())
	{
		return state_controller->GetState<Run, Player>();
	}

	return nullptr;
}
