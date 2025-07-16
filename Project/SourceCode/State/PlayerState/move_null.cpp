#include "move_null.hpp"

player_state::MoveNull::MoveNull() :
	m_non_move_time(0.0f)
{

}

player_state::MoveNull::~MoveNull()
{

}

void player_state::MoveNull::Update(Player* obj)
{
	m_non_move_time += FPS::GetDeltaTime();
}

void player_state::MoveNull::Enter(const Player* obj)
{
	m_non_move_time = 0.0f;
}

std::shared_ptr<IState<Player>> player_state::MoveNull::ChangeState(const Player* obj)
{
	// “ü—Í‚ª‚ ‚Á‚½ê‡AMove‚ÖˆÚs
	const auto command = CommandHandler::GetInstance();
	if ((	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveUpPlayer)
		||	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveDownPlayer)
		||	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveLeftPlayer)
		||	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveRightPlayer)))
	{
		return obj->GetStateController()->GetState<Move, Player>();
	}

	return nullptr;
}
