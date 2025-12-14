#include "player_run.hpp"

player_state::Run::Run(Player& player) :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kRun)),
	m_is_stop_all_state	(false),
	m_player			(player)
{

}

player_state::Run::~Run()
{

}

void player_state::Run::Update()
{
	obj->CalcMoveSpeedRun();
	obj->DirOfMovement();
}

void player_state::Run::LateUpdate()
{
	obj->OnFootIK();
}

void player_state::Run::Enter()
{

}

void player_state::Run::Exit()
{
	CommandHandler::GetInstance()->InitCurrentTriggerInputCount(CommandKind::kRun);
}

int player_state::Run::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

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
	// ステルスキル
	if (state_controller->TryStealthKill(obj))
	{
		return state_controller->GetState<StealthKill, Player>();
	}
	// 捕まれる
	if (state_controller->TryGrabbed(obj))
	{
		return state_controller->GetState<Grabbed, Player>();
	}
	// NULL
	if (!state_controller->TryRun(obj))
	{
		return state_controller->GetState<ActionNull, Player>();
	}
	// しゃがむ
	if (obj->CanControl() && command->IsExecute(CommandKind::kCrouch, TimeKind::kCurrent))
	{
		// Runコマンドがホールド方式で、入力中であった場合は移行を許可しない
		if (!(command_mode == InputModeKind::kHold && state_controller->TryRun(obj)))
		{
			return state_controller->GetState<Crouch, Player>();
		}
		command->InitCurrentTriggerInputCount(CommandKind::kCrouch);
	}

	return nullptr;
}
