#include "crouch_right_stun.hpp"

zombie_state::CrouchRightStun::CrouchRightStun() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kCrouchRightStun))
{

}

zombie_state::CrouchRightStun::~CrouchRightStun()
{

}

void zombie_state::CrouchRightStun::Update(std::shared_ptr<Zombie> obj)
{
	obj->CalcAttackIntervalTime();
}

void zombie_state::CrouchRightStun::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::CrouchRightStun::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::CrouchRightStun::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::CrouchRightStun::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	// Ž€–S
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Zombie>();
	}

	return nullptr;
}
