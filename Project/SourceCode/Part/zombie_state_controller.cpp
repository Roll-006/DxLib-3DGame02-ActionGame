#include "zombie_state_controller.hpp"

ZombieStateController::ZombieStateController()
{

}

ZombieStateController::~ZombieStateController()
{

}

void ZombieStateController::Update(std::shared_ptr<Zombie> zombie)
{

}

void ZombieStateController::LateUpdate(std::shared_ptr<Zombie> zombie)
{

}

void ZombieStateController::CreateState()
{

}

void ZombieStateController::AddStopStatePair()
{

}

void ZombieStateController::AddCheckStopState()
{

}

void ZombieStateController::ChangeState(std::shared_ptr<Zombie> zombie)
{

}

std::vector<std::shared_ptr<IState<Zombie>>> ZombieStateController::CreateChangeState(std::shared_ptr<Zombie> zombie)
{
	return std::vector<std::shared_ptr<IState<Zombie>>>();
}

std::vector<std::shared_ptr<IState<Zombie>>> ZombieStateController::CreateFutureState(const std::vector<std::shared_ptr<IState<Zombie>>>& next_state)
{
	return std::vector<std::shared_ptr<IState<Zombie>>>();
}

void ZombieStateController::StopState(std::vector<std::shared_ptr<IState<Zombie>>>& future_state, const std::shared_ptr<IState<Zombie>> stop_state)
{

}

void ZombieStateController::JudgeDestinationMoveState(std::shared_ptr<IState<Zombie>>& stop_state)
{

}

void ZombieStateController::JudgeDestinationActionState(std::shared_ptr<IState<Zombie>>& stop_state)
{

}

void ZombieStateController::JudgeDestinationWeaponActionState(std::shared_ptr<IState<Zombie>>& stop_state)
{

}
