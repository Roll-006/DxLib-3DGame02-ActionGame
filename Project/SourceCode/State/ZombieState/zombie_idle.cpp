#include "zombie_idle.hpp"

zombie_state::Idle::Idle() :
	MoveStateBase(static_cast<int>(zombie_state::MoveStateKind::kMoveNull)),
	m_is_all_stop(false)
{

}

zombie_state::Idle::~Idle()
{

}

void zombie_state::Idle::Update(std::shared_ptr<Zombie>& obj)
{
	obj->CalcMoveSpeedStop();
}

void zombie_state::Idle::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Idle::Enter(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Idle::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Idle::ChangeState(std::shared_ptr<Zombie>& obj)
{
	const auto state_controller = obj->GetStateController();

	// ˆÚ“®
	if (state_controller->TryMove())
	{
		return state_controller->GetState<Move, Zombie>();
	}

	return nullptr;
}
