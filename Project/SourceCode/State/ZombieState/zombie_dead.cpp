#include "zombie_dead.hpp"

zombie_state::Dead::Dead() :
	ActionStateBase		(static_cast<int>(zombie_state::ActionStateKind::kDead)),
	m_is_stop_all_state	(false)
{

}

zombie_state::Dead::~Dead()
{

}

void zombie_state::Dead::Update(std::shared_ptr<Zombie>& obj)
{
	obj->DisallowStealthKill();
}

void zombie_state::Dead::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Dead::Enter(std::shared_ptr<Zombie>& obj)
{
	obj->RemoveCollider(ColliderKind::kCollider);
}

void zombie_state::Dead::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Dead::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	return nullptr;
}
