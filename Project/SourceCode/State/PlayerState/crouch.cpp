#include "crouch.hpp"

player_state::Crouch::Crouch() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kCrouch)),
	m_is_stop_all_state	(false)
{

}

player_state::Crouch::~Crouch()
{

}

void player_state::Crouch::Update(std::shared_ptr<Player>& obj)
{
	//obj->CalcMoveSpeedCrouch();
	obj->DirOfCameraForward();
}

void player_state::Crouch::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::Crouch::Enter(std::shared_ptr<Player>& obj)
{

}

void player_state::Crouch::Exit(std::shared_ptr<Player>& obj)
{
	CommandHandler::GetInstance()->InitCurrentTriggerInputCount(CommandKind::kCrouch);
}

std::shared_ptr<IState<Player>> player_state::Crouch::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();
	const auto command_mode		= command->GetInstance()->GetInputModeKind(CommandKind::kCrouch);

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
	if (!command->IsExecute(CommandKind::kCrouch, TimeKind::kCurrent))
	{
		return state_controller->GetState<ActionNull, Player>();
	}
	// ダッシュ
	if (state_controller->TryRun())
	{
		// Crouchコマンドがホールド方式で、入力中であった場合は移行を許可しない
		if (!(command_mode == InputModeKind::kHold && command->IsExecute(CommandKind::kCrouch, TimeKind::kCurrent)))
		{
			return state_controller->GetState<Run, Player>();
		}
		command->InitCurrentTriggerInputCount(CommandKind::kRun);
	}

	return nullptr;
}
