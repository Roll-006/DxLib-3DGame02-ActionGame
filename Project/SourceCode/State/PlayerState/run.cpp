#include "run.hpp"

player_state::Run::Run() :
	ActionStateBase(static_cast<int>(player_state::ActionStateKind::kRun))
{

}

player_state::Run::~Run()
{

}

void player_state::Run::Update(Player* obj)
{
	obj->CalcRunSpeed();
	obj->DirOfMovement();
}

void player_state::Run::LateUpdate(Player* obj)
{

}

void player_state::Run::Enter(Player* obj)
{

}

void player_state::Run::Exit(Player* obj)
{
	CommandHandler::GetInstance()->InitTriggerCount(CommandKind::kRun);
}

std::shared_ptr<IState<Player>> player_state::Run::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();

	if (!CommandHandler::GetInstance()->IsExecutingCommand(CommandKind::kRun))
	{
		return state_controller->GetState<ActionNull, Player>();
	}
	if (CommandHandler::GetInstance()->IsExecutingCommand(CommandKind::kCrouch))
	{
		return state_controller->GetState<Crouch, Player>();
	}

	return nullptr;
}
