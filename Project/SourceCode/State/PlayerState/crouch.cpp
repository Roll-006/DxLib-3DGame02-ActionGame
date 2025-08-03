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
	obj->CalcMoveSpeedCrouch();
	obj->DirOfCameraForward();
}

void player_state::Crouch::LateUpdate(Player* obj)
{

}

void player_state::Crouch::Enter(Player* obj)
{

}

void player_state::Crouch::Exit(Player* obj)
{
	CommandHandler::GetInstance()->InitTriggerInputCount(CommandKind::kCrouch);
}

std::shared_ptr<IState<Player>> player_state::Crouch::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();
	const auto command_mode		= command->GetInstance()->GetInputModeKind(CommandKind::kCrouch);

	// NULL
	if (!command->IsExecuting(CommandKind::kCrouch))
	{
		return state_controller->GetState<ActionNull, Player>();
	}
	// ダッシュ
	if (state_controller->TryRun())
	{
		// Crouchコマンドがホールド方式で、入力中であった場合は移行を許可しない
		if (!(command_mode == InputModeKind::kHold && command->IsExecuting(CommandKind::kCrouch)))
		{
			return state_controller->GetState<Run, Player>();
		}
		command->InitTriggerInputCount(CommandKind::kRun);
	}

	return nullptr;
}
