#include "zombie_state_controller.hpp"

ZombieStateController::ZombieStateController()
{
	AttachTarget(ObjManager::GetInstance()->GetObj<CharacterBase>(ObjName.PLAYER));

	CreateState();
	AddCheckStopState();
	AddStopStatePair();

	// 初期ステート
	m_ai_state	  [TimeKind::kPrev] = m_ai_state	[TimeKind::kCurrent] = GetState<zombie_state::Wait,		  Zombie>();
	m_move_state  [TimeKind::kPrev] = m_move_state	[TimeKind::kCurrent] = GetState<zombie_state::MoveNull,   Zombie>();
	m_action_state[TimeKind::kPrev] = m_action_state[TimeKind::kCurrent] = GetState<zombie_state::ActionNull, Zombie>();
}

ZombieStateController::~ZombieStateController()
{

}

void ZombieStateController::Update(std::shared_ptr<Zombie> zombie)
{
	ChangeState(zombie);

	m_ai_state		.at(TimeKind::kCurrent)->Update(zombie);
	m_move_state	.at(TimeKind::kCurrent)->Update(zombie);
	m_action_state	.at(TimeKind::kCurrent)->Update(zombie);
}

void ZombieStateController::LateUpdate(std::shared_ptr<Zombie> zombie)
{
	m_ai_state		.at(TimeKind::kCurrent)->LateUpdate(zombie);
	m_move_state	.at(TimeKind::kCurrent)->LateUpdate(zombie);
	m_action_state	.at(TimeKind::kCurrent)->LateUpdate(zombie);
}

void ZombieStateController::CreateState()
{
	m_states[typeid(zombie_state::Wait)]				= std::make_shared<zombie_state::Wait>();
	m_states[typeid(zombie_state::Patrol)]				= std::make_shared<zombie_state::Patrol>();
	m_states[typeid(zombie_state::Investigate)]			= std::make_shared<zombie_state::Investigate>();
	m_states[typeid(zombie_state::Search)]				= std::make_shared<zombie_state::Search>();
	m_states[typeid(zombie_state::Alert)]				= std::make_shared<zombie_state::Alert>();
	m_states[typeid(zombie_state::Track)]				= std::make_shared<zombie_state::Track>();
	m_states[typeid(zombie_state::RunAttack)]			= std::make_shared<zombie_state::RunAttack>();
	m_states[typeid(zombie_state::CloseRangeAttack)]	= std::make_shared<zombie_state::CloseRangeAttack>();
	m_states[typeid(zombie_state::LongRangeAttack)]		= std::make_shared<zombie_state::LongRangeAttack>();

	m_states[typeid(zombie_state::MoveNull)]			= std::make_shared<zombie_state::MoveNull>();
	m_states[typeid(zombie_state::Move)]				= std::make_shared<zombie_state::Move>();

	m_states[typeid(zombie_state::ActionNull)]			= std::make_shared<zombie_state::ActionNull>();
	m_states[typeid(zombie_state::Run)]					= std::make_shared<zombie_state::Run>();
	m_states[typeid(zombie_state::Grab)]				= std::make_shared<zombie_state::Grab>();
	m_states[typeid(zombie_state::GrabRun)]				= std::make_shared<zombie_state::GrabRun>();
	m_states[typeid(zombie_state::Knockback)]			= std::make_shared<zombie_state::Knockback>();
	m_states[typeid(zombie_state::StandStun)]			= std::make_shared<zombie_state::StandStun>();
	m_states[typeid(zombie_state::CrouchStun)]			= std::make_shared<zombie_state::CrouchStun>();
	m_states[typeid(zombie_state::PlayDead)]			= std::make_shared<zombie_state::PlayDead>();
	m_states[typeid(zombie_state::Dead)]				= std::make_shared<zombie_state::Dead>();
}

void ZombieStateController::AddStopStatePair()
{
	m_states.at(typeid(zombie_state::Grab))->AddStopState(m_states.at(typeid(zombie_state::Move))->GetStateHandle());
}

void ZombieStateController::AddCheckStopState()
{
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(zombie_state::Move))->GetStateHandle());
}

void ZombieStateController::ChangeState(std::shared_ptr<Zombie> zombie)
{
	const auto change_state = CreateChangeState(zombie);

	if (change_state.at(0))
	{
		m_ai_state.at(TimeKind::kPrev)			= m_ai_state.at(TimeKind::kCurrent);
		m_ai_state.at(TimeKind::kCurrent)		= std::static_pointer_cast<AIStateBase<Zombie>>(change_state.at(0));
		m_ai_state.at(TimeKind::kPrev)			->Exit(zombie);
		m_ai_state.at(TimeKind::kCurrent)		->Enter(zombie);
	}

	if (change_state.at(1))
	{
		m_move_state.at(TimeKind::kPrev)		= m_move_state.at(TimeKind::kCurrent);
		m_move_state.at(TimeKind::kCurrent)		= std::static_pointer_cast<MoveStateBase<Zombie>>(change_state.at(1));
		m_move_state.at(TimeKind::kPrev)		->Exit(zombie);
		m_move_state.at(TimeKind::kCurrent)		->Enter(zombie);
	}

	if (change_state.at(2))
	{
		m_action_state.at(TimeKind::kPrev)		= m_action_state.at(TimeKind::kCurrent);
		m_action_state.at(TimeKind::kCurrent)	= std::static_pointer_cast<ActionStateBase<Zombie>>(change_state.at(2));
		m_action_state.at(TimeKind::kPrev)		->Exit(zombie);
		m_action_state.at(TimeKind::kCurrent)	->Enter(zombie);
	}
}

std::vector<std::shared_ptr<IState<Zombie>>> ZombieStateController::CreateChangeState(std::shared_ptr<Zombie> zombie)
{
	// 次変更予定のステート
	std::vector<std::shared_ptr<IState<Zombie>>> next_state
	{
		m_ai_state		.at(TimeKind::kCurrent)->ChangeState(zombie),
		m_move_state	.at(TimeKind::kCurrent)->ChangeState(zombie),
		m_action_state	.at(TimeKind::kCurrent)->ChangeState(zombie)
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
		future_state.at(0) == m_ai_state	.at(TimeKind::kCurrent) ? nullptr : future_state.at(0),
		future_state.at(1) == m_move_state	.at(TimeKind::kCurrent) ? nullptr : future_state.at(1),
		future_state.at(2) == m_action_state.at(TimeKind::kCurrent) ? nullptr : future_state.at(2)
	};
}

std::vector<std::shared_ptr<IState<Zombie>>> ZombieStateController::CreateFutureState(const std::vector<std::shared_ptr<IState<Zombie>>>& next_state)
{
	// 変更があったステートは置き換える
	return std::vector<std::shared_ptr<IState<Zombie>>>
	{
		next_state.at(0) == nullptr ? m_ai_state	.at(TimeKind::kCurrent) : next_state.at(0),
		next_state.at(1) == nullptr ? m_move_state	.at(TimeKind::kCurrent) : next_state.at(1),
		next_state.at(2) == nullptr ? m_action_state.at(TimeKind::kCurrent) : next_state.at(2)
	};
}

void ZombieStateController::StopState(std::vector<std::shared_ptr<IState<Zombie>>>& future_state, const std::shared_ptr<IState<Zombie>> stop_state)
{
	for (size_t i = 0; i < future_state.size(); ++i)
	{
		if (future_state.at(i) == stop_state)
		{
			switch (i)
			{
			case 0:
				JudgeDestinationAIState(future_state.at(i));
				break;

			case 1:
				JudgeDestinationMoveState(future_state.at(i));
				break;

			case 2:
				JudgeDestinationActionState(future_state.at(i));
				break;
			}

			return;
		}
	}
}

void ZombieStateController::JudgeDestinationAIState(std::shared_ptr<IState<Zombie>>& stop_state)
{
	switch (static_cast<zombie_state::AIStateKind>(stop_state->GetStateKind()))
	{
	default:
		break;
	}
}

void ZombieStateController::JudgeDestinationMoveState(std::shared_ptr<IState<Zombie>>& stop_state)
{
	switch (static_cast<zombie_state::MoveStateKind>(stop_state->GetStateKind()))
	{
	case zombie_state::MoveStateKind::kMove:
		stop_state = m_states.at(typeid(zombie_state::MoveNull));
		break;

	default:
		break;
	}
}

void ZombieStateController::JudgeDestinationActionState(std::shared_ptr<IState<Zombie>>& stop_state)
{
	switch (static_cast<zombie_state::ActionStateKind>(stop_state->GetStateKind()))
	{
	default:
		break;
	}
}


#pragma region Try判定
bool ZombieStateController::TryTrack(std::shared_ptr<Zombie> zombie)
{
	// TODO : 後に音などの判定も含める

	if (!m_target_character) { return false; }

	const auto is_in_sight = zombie->IsTargetInSight(m_target_character->GetModeler()->GetModelHandle());

	return is_in_sight;
}

bool ZombieStateController::TryRunAttack(std::shared_ptr<Zombie> zombie)
{
	if (!m_target_character) { return false; }

	const auto is_in_sight = zombie->IsTargetInSight(m_target_character->GetModeler()->GetModelHandle());

	return is_in_sight;
}

bool ZombieStateController::TryMove()
{
	const auto ai_state_kind	= static_cast<zombie_state::AIStateKind>(GetAIState(TimeKind::kCurrent)->GetStateKind());

	const auto is_track			= ai_state_kind == zombie_state::AIStateKind::kTrack	 ? true : false;
	const auto is_run_attack	= ai_state_kind == zombie_state::AIStateKind::kRunAttack ? true : false;

	return is_track || is_run_attack;
}

bool ZombieStateController::TryWalk(std::shared_ptr<Zombie> zombie)
{
	if (!m_target_character) { return false; }
	if (m_move_state.at(TimeKind::kCurrent)->GetStateKind() != static_cast<int>(zombie_state::MoveStateKind::kMove)) { return false; }

	const auto pos			= zombie->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto target_pos	= m_target_character->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto distance		= VSize(pos - target_pos);

	return distance < 140.0f;
}

bool ZombieStateController::TryRun(std::shared_ptr<Zombie> zombie)
{
	if (!m_target_character) { return false; }
	if (m_move_state.at(TimeKind::kCurrent)->GetStateKind() != static_cast<int>(zombie_state::MoveStateKind::kMove)) { return false; }

	const auto pos			= zombie->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto target_pos	= m_target_character->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto distance		= VSize(pos - target_pos);

	return distance > 160.0f;
}

bool ZombieStateController::TryGrabRun()
{
	const auto ai_state_kind = static_cast<zombie_state::AIStateKind>(GetAIState(TimeKind::kCurrent)->GetStateKind());

	const auto is_run_attack = ai_state_kind == zombie_state::AIStateKind::kRunAttack ? true : false;

	return is_run_attack;
}
#pragma endregion
