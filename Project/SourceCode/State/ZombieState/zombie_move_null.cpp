#include "zombie_move_null.hpp"

zombie_state::MoveNull::MoveNull() :
	MoveStateBase(static_cast<int>(zombie_state::MoveStateKind::kMoveNull))
{

}

zombie_state::MoveNull::~MoveNull()
{

}

void zombie_state::MoveNull::Update(std::shared_ptr<Zombie> obj)
{
	
}

void zombie_state::MoveNull::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::MoveNull::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::MoveNull::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::MoveNull::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	// ˆÚ“®
	if (state_controller->TryMove())
	{
		return state_controller->GetState<Move, Zombie>();
	}

	return nullptr;
}
