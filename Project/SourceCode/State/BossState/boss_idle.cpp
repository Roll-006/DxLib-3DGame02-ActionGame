#include "boss_idle.hpp"

boss_state::Idle::Idle() :
	MoveStateBase		(static_cast<int>(boss_state::MoveStateKind::kMoveNull)),
	m_is_stop_all_state	(false)
{

}

boss_state::Idle::~Idle()
{

}

void boss_state::Idle::Update(std::shared_ptr<Boss>& obj)
{
	obj->CalcMoveSpeedStop();
}

void boss_state::Idle::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Idle::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Idle::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::Idle::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// ˆÚ“®
	if (state_controller->TryMove())
	{
		return state_controller->GetState<Move, Boss>();
	}

	return nullptr;
}
