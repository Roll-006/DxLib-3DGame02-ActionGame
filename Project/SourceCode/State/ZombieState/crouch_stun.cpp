#include "crouch_stun.hpp"

zombie_state::CrouchStun::CrouchStun() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kActionNull))
{

}

zombie_state::CrouchStun::~CrouchStun()
{

}

void zombie_state::CrouchStun::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::CrouchStun::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::CrouchStun::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::CrouchStun::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::CrouchStun::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
