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
	CommandHandler::GetInstance()->InitTriggerInputCount(CommandKind::kRun);
}

std::shared_ptr<IState<Player>> player_state::Run::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();
	const auto command_mode		= command->GetInstance()->GetInputModeKind(CommandKind::kRun);

	// NULL
	if (!state_controller->TryRun())
	{
		return state_controller->GetState<ActionNull, Player>();
	}
	// しゃがむ
	if (command->IsExecuting(CommandKind::kCrouch))
	{
		// Runコマンドがホールド方式で、入力中であった場合は移行を許可しない
		if (!(command_mode == InputModeKind::kHold && state_controller->TryRun()))
		{
			return state_controller->GetState<Crouch, Player>();
		}
		command->InitTriggerInputCount(CommandKind::kCrouch);
	}

	return nullptr;
}
