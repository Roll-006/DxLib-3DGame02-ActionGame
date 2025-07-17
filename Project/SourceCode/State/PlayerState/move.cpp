#include "move.hpp"

player_state::Move::Move() :
	MoveStateBase(static_cast<int>(player_state::MoveStateKind::kMove))
{

}

player_state::Move::~Move()
{

}

void player_state::Move::Update(Player* obj)
{
	obj->Move();
}

void player_state::Move::Enter(Player* obj)
{

}

void player_state::Move::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::Move::ChangeState(const Player* obj)
{
	// ‰½‚à“ü—Í‚³‚ê‚Ä‚¢‚È‚¢ê‡ANull(Idle)‚ÖˆÚs
	if (!obj->GetStateController()->TryMove())
	{
		return obj->GetStateController()->GetState<MoveNull, Player>();
	}

	return nullptr;
}
