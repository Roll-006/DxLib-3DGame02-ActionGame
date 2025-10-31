#include "detected.hpp"

zombie_state::Detected::Detected() :
	MoveStateBase		(static_cast<int>(zombie_state::ActionStateKind::kDetected)),
	m_is_stop_all_state	(false)
{

}

zombie_state::Detected::~Detected()
{

}

void zombie_state::Detected::Update(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Detected::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Detected::Enter(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Detected::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Detected::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();



	return nullptr;
}
