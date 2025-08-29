#include "zombie_state_controller.hpp"

ZombieStateController::ZombieStateController()
{
	CreateState();
	AddCheckStopState();
	AddStopStatePair();

	// 初期ステート
	m_action_state[TimeKind::kPrev] = m_action_state[TimeKind::kCurrent] = GetState<zombie_state::ActionNull, Zombie>();
}

ZombieStateController::~ZombieStateController()
{

}

void ZombieStateController::Update(std::shared_ptr<Zombie> zombie)
{
	ChangeState(zombie);

	m_action_state.at(TimeKind::kCurrent)->Update(zombie);
}

void ZombieStateController::LateUpdate(std::shared_ptr<Zombie> zombie)
{
	m_action_state.at(TimeKind::kCurrent)->LateUpdate(zombie);
}

void ZombieStateController::CreateState()
{
	m_states[typeid(zombie_state::ActionNull)]	= std::make_shared<zombie_state::ActionNull>();
	m_states[typeid(zombie_state::Grab)]		= std::make_shared<zombie_state::Grab>();
	m_states[typeid(zombie_state::StandStun)]	= std::make_shared<zombie_state::StandStun>();
	m_states[typeid(zombie_state::CrouchStun)]	= std::make_shared<zombie_state::CrouchStun>();
	m_states[typeid(zombie_state::PlayDead)]	= std::make_shared<zombie_state::PlayDead>();
	m_states[typeid(zombie_state::Dead)]		= std::make_shared<zombie_state::Dead>();
}

void ZombieStateController::AddStopStatePair()
{

}

void ZombieStateController::AddCheckStopState()
{

}

void ZombieStateController::ChangeState(std::shared_ptr<Zombie> zombie)
{
	const auto change_state = CreateChangeState(zombie);

	if (change_state.at(0))
	{
		m_action_state.at(TimeKind::kPrev)		= m_action_state.at(TimeKind::kCurrent);
		m_action_state.at(TimeKind::kCurrent)	= std::static_pointer_cast<ActionStateBase<Zombie>>(change_state.at(0));
		m_action_state.at(TimeKind::kPrev)		->Exit(zombie);
		m_action_state.at(TimeKind::kCurrent)	->Enter(zombie);
	}
}

std::vector<std::shared_ptr<IState<Zombie>>> ZombieStateController::CreateChangeState(std::shared_ptr<Zombie> zombie)
{
	// 次変更予定のステート
	std::vector<std::shared_ptr<IState<Zombie>>> next_state
	{
		m_action_state.at(TimeKind::kCurrent)->ChangeState(zombie)
	};

	std::vector<int> check_stop_state_index;
	auto future_state = CreateFutureState(next_state);

	// 上の階層にあるステートの停止処理
	for (int i = future_state.size() - 1; i >= 0; --i)
	{
		// 停止判定
		for (auto itr = check_stop_state_index.begin(); itr != check_stop_state_index.end(); )
		{
			if (future_state.at(i)->IsStop(future_state.at(*itr)->GetStateHandle()))
			{
				// ステートを停止させ未来のステートに反映
				StopState(future_state, future_state.at(*itr));

				itr = check_stop_state_index.erase(itr);
			}
			else
			{
				++itr;
			}
		}

		// 自身が停止される対象かを判定
		if (std::find(m_check_stop_state_handles.begin(), m_check_stop_state_handles.end(), future_state.at(i)->GetStateHandle()) != m_check_stop_state_handles.end())
		{
			check_stop_state_index.emplace_back(i);
		}
	}

	// 下の階層にあるステートの停止処理
	for (size_t i = 0; i < future_state.size(); ++i)
	{
		// 停止判定
		for (auto itr = check_stop_state_index.begin(); itr != check_stop_state_index.end(); )
		{
			if (future_state.at(i)->IsStop(future_state.at(*itr)->GetStateHandle()))
			{
				// ステートを停止させ未来のステートに反映
				StopState(future_state, future_state.at(*itr));

				itr = check_stop_state_index.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}

	// 変更があったステートのみを取得
	// 変更がない場合はnullptr
	return std::vector<std::shared_ptr<IState<Zombie>>>
	{
		future_state.at(0) == m_action_state.at(TimeKind::kCurrent) ? nullptr : future_state.at(0)
	};
}

std::vector<std::shared_ptr<IState<Zombie>>> ZombieStateController::CreateFutureState(const std::vector<std::shared_ptr<IState<Zombie>>>& next_state)
{
	// 変更があったステートは置き換える
	return std::vector<std::shared_ptr<IState<Zombie>>>
	{
		next_state.at(0) == nullptr ? m_action_state.at(TimeKind::kCurrent) : next_state.at(0)
	};
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
