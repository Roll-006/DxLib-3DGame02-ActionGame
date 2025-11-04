#include "boss_state_controller.hpp"

BossStateController::BossStateController()
{
	CreateState();
	AddCheckStopState();
	AddStopStatePair();

	// 初期ステート
	m_ai_state	  [TimeKind::kPrev] = m_ai_state	[TimeKind::kCurrent] = GetState<boss_state::Wait,		Boss>();
	m_move_state  [TimeKind::kPrev] = m_move_state	[TimeKind::kCurrent] = GetState<boss_state::Idle,		Boss>();
	m_action_state[TimeKind::kPrev] = m_action_state[TimeKind::kCurrent] = GetState<boss_state::ActionNull, Boss>();
}

BossStateController::~BossStateController()
{

}

void BossStateController::Init(std::shared_ptr<Boss> boss)
{
	m_ai_state		.at(TimeKind::kCurrent)->Exit(boss);
	m_move_state	.at(TimeKind::kCurrent)->Exit(boss);
	m_action_state	.at(TimeKind::kCurrent)->Exit(boss);
}

void BossStateController::Update(std::shared_ptr<Boss> boss)
{
	ChangeState(boss);

	m_ai_state		.at(TimeKind::kCurrent)->Update(boss);
	m_move_state	.at(TimeKind::kCurrent)->Update(boss);
	m_action_state	.at(TimeKind::kCurrent)->Update(boss);
}

void BossStateController::LateUpdate(std::shared_ptr<Boss> boss)
{
	m_ai_state		.at(TimeKind::kCurrent)->LateUpdate(boss);
	m_move_state	.at(TimeKind::kCurrent)->LateUpdate(boss);
	m_action_state	.at(TimeKind::kCurrent)->LateUpdate(boss);
}

void BossStateController::CreateState()
{
	m_states[typeid(boss_state::Wait)]				= std::make_shared<boss_state::Wait>();
	m_states[typeid(boss_state::Patrol)]			= std::make_shared<boss_state::Patrol>();
	m_states[typeid(boss_state::Investigate)]		= std::make_shared<boss_state::Investigate>();
	m_states[typeid(boss_state::Search)]			= std::make_shared<boss_state::Search>();
	m_states[typeid(boss_state::Alert)]				= std::make_shared<boss_state::Alert>();
	m_states[typeid(boss_state::Track)]				= std::make_shared<boss_state::Track>();
	m_states[typeid(boss_state::RunAttack)]			= std::make_shared<boss_state::RunAttack>();
	m_states[typeid(boss_state::CloseRangeAttack)]	= std::make_shared<boss_state::CloseRangeAttack>();
	m_states[typeid(boss_state::LongRangeAttack)]	= std::make_shared<boss_state::LongRangeAttack>();

	m_states[typeid(boss_state::Idle)]				= std::make_shared<boss_state::Idle>();
	m_states[typeid(boss_state::Move)]				= std::make_shared<boss_state::Move>();

	m_states[typeid(boss_state::ActionNull)]		= std::make_shared<boss_state::ActionNull>();
	m_states[typeid(boss_state::Detected)]			= std::make_shared<boss_state::Detected>();
	m_states[typeid(boss_state::Run)]				= std::make_shared<boss_state::Run>();
	m_states[typeid(boss_state::StealthKilled)]		= std::make_shared<boss_state::StealthKilled>();
	m_states[typeid(boss_state::Knockback)]			= std::make_shared<boss_state::Knockback>();
	m_states[typeid(boss_state::BackwardKnockback)]	= std::make_shared<boss_state::BackwardKnockback>();
	m_states[typeid(boss_state::StandUp)]			= std::make_shared<boss_state::StandUp>();
	m_states[typeid(boss_state::StandStun)]			= std::make_shared<boss_state::StandStun>();
	m_states[typeid(boss_state::CrouchLeftStun)]	= std::make_shared<boss_state::CrouchLeftStun>();
	m_states[typeid(boss_state::CrouchRightStun)]	= std::make_shared<boss_state::CrouchRightStun>();
	m_states[typeid(boss_state::PlayDead)]			= std::make_shared<boss_state::PlayDead>();
	m_states[typeid(boss_state::Dead)]				= std::make_shared<boss_state::Dead>();
}

void BossStateController::AddStopStatePair()
{
	m_states.at(typeid(boss_state::Detected))		->AddStopState(m_states.at(typeid(boss_state::Move))->GetStateHandle());
	m_states.at(typeid(boss_state::StandStun))		->AddStopState(m_states.at(typeid(boss_state::Move))->GetStateHandle());
	m_states.at(typeid(boss_state::CrouchLeftStun))	->AddStopState(m_states.at(typeid(boss_state::Move))->GetStateHandle());
	m_states.at(typeid(boss_state::CrouchRightStun))->AddStopState(m_states.at(typeid(boss_state::Move))->GetStateHandle());
	m_states.at(typeid(boss_state::Dead))			->AddStopState(m_states.at(typeid(boss_state::Move))->GetStateHandle());
}

void BossStateController::AddCheckStopState()
{
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(boss_state::Move))->GetStateHandle());
}

void BossStateController::ChangeState(std::shared_ptr<Boss>& boss)
{
	const auto change_state = CreateChangeState(boss);

	if (change_state.at(0))
	{
		m_ai_state.at(TimeKind::kPrev)			= m_ai_state.at(TimeKind::kCurrent);
		m_ai_state.at(TimeKind::kCurrent)		= std::static_pointer_cast<AIStateBase<Boss>>(change_state.at(0));
		m_ai_state.at(TimeKind::kPrev)			->Exit(boss);
		m_ai_state.at(TimeKind::kCurrent)		->Enter(boss);
	}

	if (change_state.at(1))
	{
		m_move_state.at(TimeKind::kPrev)		= m_move_state.at(TimeKind::kCurrent);
		m_move_state.at(TimeKind::kCurrent)		= std::static_pointer_cast<MoveStateBase<Boss>>(change_state.at(1));
		m_move_state.at(TimeKind::kPrev)		->Exit(boss);
		m_move_state.at(TimeKind::kCurrent)		->Enter(boss);
	}

	if (change_state.at(2))
	{
		m_action_state.at(TimeKind::kPrev)		= m_action_state.at(TimeKind::kCurrent);
		m_action_state.at(TimeKind::kCurrent)	= std::static_pointer_cast<ActionStateBase<Boss>>(change_state.at(2));
		m_action_state.at(TimeKind::kPrev)		->Exit(boss);
		m_action_state.at(TimeKind::kCurrent)	->Enter(boss);
	}
}

std::vector<std::shared_ptr<IState<Boss>>> BossStateController::CreateChangeState(std::shared_ptr<Boss>& boss)
{
	// 次変更予定のステート
	std::vector<std::shared_ptr<IState<Boss>>> next_state
	{
		m_ai_state		.at(TimeKind::kCurrent)->ChangeState(boss),
		m_move_state	.at(TimeKind::kCurrent)->ChangeState(boss),
		m_action_state	.at(TimeKind::kCurrent)->ChangeState(boss)
	};

	std::vector<int> check_stop_state_index;
	auto future_state = CreateFutureState(next_state);

	// 上の階層にあるステートの停止処理
	for (int i = future_state.size() - 1; i >= 0; --i)
	{
		// 停止判定
		for (auto itr = check_stop_state_index.begin(); itr != check_stop_state_index.end(); )
		{
			if (future_state.at(i)->IsStop(future_state.at(*itr)->GetStateHandle()) || future_state.at(i)->IsStopAllState())
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
			if (future_state.at(i)->IsStop(future_state.at(*itr)->GetStateHandle()) || future_state.at(i)->IsStopAllState())
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
	return std::vector<std::shared_ptr<IState<Boss>>>
	{
		future_state.at(0) == m_ai_state	.at(TimeKind::kCurrent) ? nullptr : future_state.at(0),
		future_state.at(1) == m_move_state	.at(TimeKind::kCurrent) ? nullptr : future_state.at(1),
		future_state.at(2) == m_action_state.at(TimeKind::kCurrent) ? nullptr : future_state.at(2)
	};
}

std::vector<std::shared_ptr<IState<Boss>>> BossStateController::CreateFutureState(const std::vector<std::shared_ptr<IState<Boss>>>& next_state)
{
	// 変更があったステートは置き換える
	return std::vector<std::shared_ptr<IState<Boss>>>
	{
		next_state.at(0) == nullptr ? m_ai_state	.at(TimeKind::kCurrent) : next_state.at(0),
		next_state.at(1) == nullptr ? m_move_state	.at(TimeKind::kCurrent) : next_state.at(1),
		next_state.at(2) == nullptr ? m_action_state.at(TimeKind::kCurrent) : next_state.at(2)
	};
}

void BossStateController::StopState(std::vector<std::shared_ptr<IState<Boss>>>& future_state, const std::shared_ptr<IState<Boss>>& stop_state)
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

void BossStateController::JudgeDestinationAIState(std::shared_ptr<IState<Boss>>& stop_state)
{
	switch (static_cast<boss_state::AIStateKind>(stop_state->GetStateKind()))
	{
	default:
		break;
	}
}

void BossStateController::JudgeDestinationMoveState(std::shared_ptr<IState<Boss>>& stop_state)
{
	switch (static_cast<boss_state::MoveStateKind>(stop_state->GetStateKind()))
	{
	case boss_state::MoveStateKind::kMove:
		stop_state = m_states.at(typeid(boss_state::Idle));
		break;

	default:
		break;
	}
}

void BossStateController::JudgeDestinationActionState(std::shared_ptr<IState<Boss>>& stop_state)
{
	switch (static_cast<boss_state::ActionStateKind>(stop_state->GetStateKind()))
	{
	default:
		break;
	}
}


#pragma region Try判定
bool BossStateController::TryWaitForcibly(std::shared_ptr<Boss>& boss)
{
	return !boss->CanAction();
}

bool BossStateController::TryPatrol(std::shared_ptr<Boss>& boss)
{
	const auto route_giver = boss->GetPatrolRouteGiver();

	if (!route_giver)				{ return false; }
	if (route_giver->IsEnd())		{ return false; }
	if (boss->IsDetectedTarget())	{ return false; }

	return true;
}

bool BossStateController::TryTrack(std::shared_ptr<Boss>& boss)
{
	// TODO : 後に音などの判定も含める

	if (!m_target_character) { return false; }

	const auto is_in_sight = boss->IsDetectedTarget();

	return is_in_sight;
}

bool BossStateController::TryRunAttack(std::shared_ptr<Boss>& boss)
{
	if (!m_target_character) { return false; }

	const auto is_in_sight	= boss->IsDetectedTarget();
	const auto can_attack	= boss->CanAttack();

	return is_in_sight && can_attack;
}

bool BossStateController::TryMove()
{
	const auto ai_state_kind	= static_cast<boss_state::AIStateKind>(GetAIState(TimeKind::kCurrent)->GetStateKind());

	const auto is_patrol		= ai_state_kind == boss_state::AIStateKind::kPatrol	 ? true : false;
	const auto is_track			= ai_state_kind == boss_state::AIStateKind::kTrack	 ? true : false;
	const auto is_run_attack	= ai_state_kind == boss_state::AIStateKind::kRunAttack ? true : false;

	return is_track || is_run_attack || is_patrol;
}

bool BossStateController::TryActionNullForcibly(std::shared_ptr<Boss>& boss)
{
	return !boss->CanAction();
}

bool BossStateController::TryDetected(std::shared_ptr<Boss>& boss)
{
	return boss->IsDetectedTarget() && !boss->IsPrevDetectedTarget();
}

bool BossStateController::TryWalk(std::shared_ptr<Boss>& boss)
{
	if (!m_target_character) { return false; }
	if (m_move_state.at(TimeKind::kCurrent)->GetStateKind() != static_cast<int>(boss_state::MoveStateKind::kMove)) { return false; }

	const auto pos			= boss->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto target_pos	= m_target_character->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto distance		= VSize(pos - target_pos);

	return distance < 140.0f;
}

bool BossStateController::TryRun(std::shared_ptr<Boss>& boss)
{
	if (!boss->IsDetectedTarget()) { return false; }
	if (!m_target_character)		{ return false; }
	if (m_move_state.at(TimeKind::kCurrent)->GetStateKind() != static_cast<int>(boss_state::MoveStateKind::kMove)) { return false; }

	const auto pos			= boss->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto target_pos	= m_target_character->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto distance		= VSize(pos - target_pos);

	// TODO : 後に定数化
	return distance > 160.0f;
}

bool BossStateController::TryStealthKilled(std::shared_ptr<Boss>& boss)
{
	return boss->IsStealthKilled();
}

bool BossStateController::TryGrabRun()
{
	const auto ai_state_kind = static_cast<boss_state::AIStateKind>(GetAIState(TimeKind::kCurrent)->GetStateKind());

	const auto is_run_attack = ai_state_kind == boss_state::AIStateKind::kRunAttack ? true : false;

	return is_run_attack;
}

bool BossStateController::TryKnockback(std::shared_ptr<Boss>& boss)
{
	return boss->GetKnockBackSpeed() > 0.0f;
}

bool BossStateController::TryDead(std::shared_ptr<Boss>& boss)
{
	return boss->GetHealth(HealthPartKind::kMain)->GetCurrentValue() <= 0.0f;
}

bool BossStateController::TryLeftCrouchStun(std::shared_ptr<Boss>& boss)
{
	return boss->GetHealth(HealthPartKind::kLeftLeg)->GetCurrentValue() <= 0.0f;
}

bool BossStateController::TryRightCrouchStun(std::shared_ptr<Boss>& boss)
{
	return boss->GetHealth(HealthPartKind::kRightLeg)->GetCurrentValue() <= 0.0f;
}

bool BossStateController::TryStandStun(std::shared_ptr<Boss>& boss)
{
	return boss->GetHealth(HealthPartKind::kHead)->GetCurrentValue() <= 0.0f;;
}
#pragma endregion
