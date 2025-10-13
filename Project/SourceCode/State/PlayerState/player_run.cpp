#include "player_run.hpp"

player_state::Run::Run() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kRun)),
	m_is_stop_all_state	(false)
{

}

player_state::Run::~Run()
{

}

void player_state::Run::Update(std::shared_ptr<Player>& obj)
{
	obj->CalcMoveSpeedRun();
	obj->DirOfMovement();
}

void player_state::Run::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::Run::Enter(std::shared_ptr<Player>& obj)
{

}

void player_state::Run::Exit(std::shared_ptr<Player>& obj)
{
	CommandHandler::GetInstance()->InitCurrentTriggerInputCount(CommandKind::kRun);
}

std::shared_ptr<IState<Player>> player_state::Run::ChangeState(std::shared_ptr<Player>& obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();
	const auto command_mode		= command->GetInstance()->GetInputModeKind(CommandKind::kRun);

	// 死亡
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Player>();
	}
	// メレー(正面蹴り)
	if (state_controller->TryFrontKick(obj))
	{
		return state_controller->GetState<FrontKick, Player>();
	}
	// メレー(回し蹴り)
	if (state_controller->TryRoundhouseKick(obj))
	{
		return state_controller->GetState<RoundhouseKick, Player>();
	}
	// 捕まれる
	if (state_controller->TryGrabbed(obj))
	{
		return state_controller->GetState<Grabbed, Player>();
	}
	// NULL
	if (!state_controller->TryRun())
	{
		return state_controller->GetState<ActionNull, Player>();
	}
	// しゃがむ
	if (command->IsExecute(CommandKind::kCrouch, TimeKind::kCurrent))
	{
		// Runコマンドがホールド方式で、入力中であった場合は移行を許可しない
		if (!(command_mode == InputModeKind::kHold && state_controller->TryRun()))
		{
			return state_controller->GetState<Crouch, Player>();
		}
		command->InitCurrentTriggerInputCount(CommandKind::kCrouch);
	}

	return nullptr;
}
