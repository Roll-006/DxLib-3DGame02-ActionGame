#include "player_action_null.hpp"

player_state::ActionNull::ActionNull() : 
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kActionNull)),
	m_is_stop_all_state	(false)
{

}

player_state::ActionNull::~ActionNull()
{

}

void player_state::ActionNull::Update(std::shared_ptr<Player>& obj)
{
	const auto state_controller = obj->GetStateController();
	if (state_controller->GetMoveState(TimeKind::kCurrent)->GetStateKind() == static_cast<int>(player_state::MoveStateKind::kMove))
	{
		obj->DirOfCameraForward();
	}
}

void player_state::ActionNull::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::ActionNull::Enter(std::shared_ptr<Player>& obj)
{

}

void player_state::ActionNull::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::ActionNull::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// 勝利ポーズ
	if (obj->IsVictoryPose())
	{
		return state_controller->GetState<VictoryPose, Player>();
	}
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
	// ダッシュ
	if (state_controller->TryRun(obj))
	{
		return state_controller->GetState<Run, Player>();
	}
	// しゃがむ
	if (obj->CanControl() && command->IsExecute(CommandKind::kCrouch, TimeKind::kCurrent))
	{
		return state_controller->GetState<Crouch, Player>();
	}

	return nullptr;
}
