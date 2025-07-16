#include "action_null.hpp"

player_state::ActionNull::ActionNull()
{

}

player_state::ActionNull::~ActionNull()
{

}

void player_state::ActionNull::Update(Player* obj)
{

}

void player_state::ActionNull::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::ActionNull::ChangeState(const Player* obj)
{
	if (TryRun())
	{
		return obj->GetStateController()->GetState<Run, Player>();
	}
	if (TryCrouch())
	{
		return obj->GetStateController()->GetState<Crouch, Player>();
	}
}

bool player_state::ActionNull::TryRun()
{
	const auto command	= CommandHandler::GetInstance();
	const auto input	= InputChecker	::GetInstance();
	const auto code		= *command->GetCurrentFrameExecuteInputCode(CommandKind::kRun);

	switch (command->GetInputModeKind(CommandHandler::MoveKind::kRun))
	{
	case InputModeKind::kTrigger:
		// トリガーの入力回数が偶数回の場合、Runへの移行を許可
		if (input->GetInputState(code) == InputState::kSingle)
		{
			command->CountUpTrigger(CommandHandler::MoveKind::kRun);

			return command->GetTriggerCount(CommandHandler::MoveKind::kRun) % 2 ? true : false;
		}
		break;

	case InputModeKind::kHold:
		// ホールド中の場合、Runへの移行を許可
		if (input->GetInputState(code) == InputState::kHold)
		{
			return true;
		}
		break;
	}

	return false;
}

bool player_state::ActionNull::TryCrouch()
{
	const auto command	= CommandHandler::GetInstance();
	const auto input	= InputChecker	::GetInstance();
	const auto code		= *command->GetCurrentFrameExecuteInputCode(CommandKind::kCrouch);

	switch (command->GetInputModeKind(CommandHandler::MoveKind::kCrouch))
	{
	case InputModeKind::kTrigger:
		// トリガーの入力回数が偶数回の場合、Crouchへの移行を許可
		if (input->GetInputState(code) == InputState::kSingle)
		{
			command->CountUpTrigger(CommandHandler::MoveKind::kCrouch);

			return command->GetTriggerCount(CommandHandler::MoveKind::kCrouch) % 2 ? true : false;
		}
		break;

	case InputModeKind::kHold:
		// ホールド中の場合、Crouchへの移行を許可
		if (input->GetInputState(code) == InputState::kHold)
		{
			return true;
		}
		break;
	}

	return false;
}
