#include "player_state_controller.hpp"

PlayerStateController::PlayerStateController()
{
	CreateState();
	AddCheckStopState();
	AddStopStatePair();

	// 初期ステート
	m_move_state		 [TimeKind::kPrev] = m_move_state		  [TimeKind::kCurrent] = GetState<player_state::MoveNull,			Player>();
	m_action_state		 [TimeKind::kPrev] = m_action_state		  [TimeKind::kCurrent] = GetState<player_state::ActionNull,			Player>();
	m_weapon_action_state[TimeKind::kPrev] = m_weapon_action_state[TimeKind::kCurrent] = GetState<player_state::WeaponActionNull,	Player>();
	m_special_state		 [TimeKind::kPrev] = m_special_state	  [TimeKind::kCurrent] = GetState<player_state::SpecialNull,		Player>();
}

PlayerStateController::~PlayerStateController()
{

}

void PlayerStateController::Update(Player* player)
{
	ChangeState(player);

	m_move_state			.at(TimeKind::kCurrent)->Update(player);
	m_action_state			.at(TimeKind::kCurrent)->Update(player);
	m_weapon_action_state	.at(TimeKind::kCurrent)->Update(player);
	m_special_state			.at(TimeKind::kCurrent)->Update(player);
}

void PlayerStateController::LateUpdate(Player* player)
{
	m_move_state			.at(TimeKind::kCurrent)->LateUpdate(player);
	m_action_state			.at(TimeKind::kCurrent)->LateUpdate(player);
	m_weapon_action_state	.at(TimeKind::kCurrent)->LateUpdate(player);
	m_special_state			.at(TimeKind::kCurrent)->LateUpdate(player);
}

void PlayerStateController::CreateState()
{
	m_states[typeid(player_state::MoveNull)]			 = std::make_shared<player_state::MoveNull>();
	m_states[typeid(player_state::Move)]				 = std::make_shared<player_state::Move>();
	m_states[typeid(player_state::ActionNull)]			 = std::make_shared<player_state::ActionNull>();
	m_states[typeid(player_state::Crouch)]				 = std::make_shared<player_state::Crouch>();
	m_states[typeid(player_state::Run)]					 = std::make_shared<player_state::Run>();
	m_states[typeid(player_state::TurnAround)]			 = std::make_shared<player_state::TurnAround>();
	m_states[typeid(player_state::CrouchTurnAround)]	 = std::make_shared<player_state::CrouchTurnAround>();
	m_states[typeid(player_state::WeaponActionNull)]	 = std::make_shared<player_state::WeaponActionNull>();
	m_states[typeid(player_state::EquipKnife)]			 = std::make_shared<player_state::EquipKnife>();
	m_states[typeid(player_state::AimKnife)]			 = std::make_shared<player_state::AimKnife>();
	m_states[typeid(player_state::StabKnife)]			 = std::make_shared<player_state::StabKnife>();
	m_states[typeid(player_state::FirstSideSlashKnife)]	 = std::make_shared<player_state::FirstSideSlashKnife>();
	m_states[typeid(player_state::SecondSideSlashKnife)] = std::make_shared<player_state::SecondSideSlashKnife>();
	m_states[typeid(player_state::Parry)]				 = std::make_shared<player_state::Parry>();
	m_states[typeid(player_state::EquipGun)]			 = std::make_shared<player_state::EquipGun>();
	m_states[typeid(player_state::AimGun)]				 = std::make_shared<player_state::AimGun>();
	m_states[typeid(player_state::Shot)]				 = std::make_shared<player_state::Shot>();
	m_states[typeid(player_state::Reload)]				 = std::make_shared<player_state::Reload>();
	m_states[typeid(player_state::SpecialNull)]			 = std::make_shared<player_state::SpecialNull>();
	m_states[typeid(player_state::RoundhouseKick)]		 = std::make_shared<player_state::RoundhouseKick>();
	m_states[typeid(player_state::FrontKick)]			 = std::make_shared<player_state::FrontKick>();
	m_states[typeid(player_state::Suplex)]				 = std::make_shared<player_state::Suplex>();
	m_states[typeid(player_state::Escape)]				 = std::make_shared<player_state::Escape>();
	m_states[typeid(player_state::StealthKill)]			 = std::make_shared<player_state::StealthKill>();
	m_states[typeid(player_state::FinishOff)]			 = std::make_shared<player_state::FinishOff>();
	m_states[typeid(player_state::EscapeWithKnife)]		 = std::make_shared<player_state::EscapeWithKnife>();
}

void PlayerStateController::AddStopStatePair()
{
	m_states.at(typeid(player_state::AimKnife))				->AddStopState(m_states.at(typeid(player_state::Crouch))->GetStateHandle());
	m_states.at(typeid(player_state::AimKnife))				->AddStopState(m_states.at(typeid(player_state::Run))	->GetStateHandle());
	m_states.at(typeid(player_state::StabKnife))			->AddStopState(m_states.at(typeid(player_state::Crouch))->GetStateHandle());
	m_states.at(typeid(player_state::StabKnife))			->AddStopState(m_states.at(typeid(player_state::Run))	->GetStateHandle());
	m_states.at(typeid(player_state::StabKnife))			->AddStopState(m_states.at(typeid(player_state::Move))	->GetStateHandle());
	m_states.at(typeid(player_state::StabKnife))			->AddStopState(m_states.at(typeid(player_state::Crouch))->GetStateHandle());
	m_states.at(typeid(player_state::StabKnife))			->AddStopState(m_states.at(typeid(player_state::Run))	->GetStateHandle());
	m_states.at(typeid(player_state::FirstSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Move))	->GetStateHandle());
	m_states.at(typeid(player_state::FirstSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Crouch))->GetStateHandle());
	m_states.at(typeid(player_state::FirstSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Run))	->GetStateHandle());
	m_states.at(typeid(player_state::SecondSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Move))	->GetStateHandle());
	m_states.at(typeid(player_state::SecondSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Crouch))->GetStateHandle());
	m_states.at(typeid(player_state::SecondSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Run))	->GetStateHandle());
	m_states.at(typeid(player_state::Parry))				->AddStopState(m_states.at(typeid(player_state::Crouch))->GetStateHandle());
	m_states.at(typeid(player_state::Parry))				->AddStopState(m_states.at(typeid(player_state::Run))	->GetStateHandle());
	m_states.at(typeid(player_state::AimGun))				->AddStopState(m_states.at(typeid(player_state::Crouch))->GetStateHandle());
	m_states.at(typeid(player_state::AimGun))				->AddStopState(m_states.at(typeid(player_state::Run))	->GetStateHandle());
	m_states.at(typeid(player_state::Shot))					->AddStopState(m_states.at(typeid(player_state::Crouch))->GetStateHandle());
	m_states.at(typeid(player_state::Shot))					->AddStopState(m_states.at(typeid(player_state::Run))	->GetStateHandle());
}

void PlayerStateController::AddCheckStopState()
{
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::Move))		->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::Crouch))	->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::Run))		->GetStateHandle());
}

void PlayerStateController::ChangeState(Player* player)
{
	const auto change_state = CreateChangeState(player);

	if (change_state.at(0))
	{
		m_move_state			.at(TimeKind::kPrev)	= m_move_state.at(TimeKind::kCurrent);
		m_move_state			.at(TimeKind::kCurrent)	= std::static_pointer_cast<MoveStateBase<Player>>(change_state.at(0));
		m_move_state			.at(TimeKind::kPrev)	->Exit (player);
		m_move_state			.at(TimeKind::kCurrent)	->Enter(player);
	}

	if (change_state.at(1))
	{
		m_action_state			.at(TimeKind::kPrev)	= m_action_state.at(TimeKind::kCurrent);
		m_action_state			.at(TimeKind::kCurrent) = std::static_pointer_cast<ActionStateBase<Player>>(change_state.at(1));
		m_action_state			.at(TimeKind::kPrev)	->Exit (player);
		m_action_state			.at(TimeKind::kCurrent) ->Enter(player);
	}

	if (change_state.at(2))
	{
		m_weapon_action_state	.at(TimeKind::kPrev)	= m_weapon_action_state.at(TimeKind::kCurrent);
		m_weapon_action_state	.at(TimeKind::kCurrent) = std::static_pointer_cast<WeaponActionStateBase<Player>>(change_state.at(2));
		m_weapon_action_state	.at(TimeKind::kPrev)	->Exit (player);
		m_weapon_action_state	.at(TimeKind::kCurrent) ->Enter(player);
	}

	if (change_state.at(3))
	{
		m_special_state			.at(TimeKind::kPrev)	= m_special_state.at(TimeKind::kCurrent);
		m_special_state			.at(TimeKind::kCurrent) = std::static_pointer_cast<SpecialStateBase<Player>>(change_state.at(3));
		m_special_state			.at(TimeKind::kPrev)	->Exit (player);
		m_special_state			.at(TimeKind::kCurrent) ->Enter(player);
	}
}

std::vector<std::shared_ptr<IState<Player>>> PlayerStateController::CreateChangeState(Player* player)
{
	// 次変更予定のステート
	std::vector<std::shared_ptr<IState<Player>>> next_state
	{
		m_move_state		 .at(TimeKind::kCurrent)->ChangeState(player),
		m_action_state		 .at(TimeKind::kCurrent)->ChangeState(player),
		m_weapon_action_state.at(TimeKind::kCurrent)->ChangeState(player),
		m_special_state		 .at(TimeKind::kCurrent)->ChangeState(player)
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
	return std::vector<std::shared_ptr<IState<Player>>>
	{
		future_state.at(0) == m_move_state			.at(TimeKind::kCurrent) ? nullptr : future_state.at(0),
		future_state.at(1) == m_action_state		.at(TimeKind::kCurrent) ? nullptr : future_state.at(1),
		future_state.at(2) == m_weapon_action_state	.at(TimeKind::kCurrent) ? nullptr : future_state.at(2),
		future_state.at(3) == m_special_state		.at(TimeKind::kCurrent) ? nullptr : future_state.at(3)
	};
}

std::vector<std::shared_ptr<IState<Player>>> PlayerStateController::CreateFutureState(const std::vector<std::shared_ptr<IState<Player>>>& next_state)
{
	// 変更があったステートは置き換える
	return std::vector<std::shared_ptr<IState<Player>>>
	{
		next_state.at(0) == nullptr ? m_move_state			.at(TimeKind::kCurrent) : next_state.at(0),
		next_state.at(1) == nullptr ? m_action_state		.at(TimeKind::kCurrent) : next_state.at(1),
		next_state.at(2) == nullptr ? m_weapon_action_state	.at(TimeKind::kCurrent) : next_state.at(2),
		next_state.at(3) == nullptr ? m_special_state		.at(TimeKind::kCurrent) : next_state.at(3)
	};
}

void PlayerStateController::StopState(std::vector<std::shared_ptr<IState<Player>>>& future_state, const std::shared_ptr<IState<Player>> stop_state)
{
	for (size_t i = 0; i < future_state.size(); ++i)
	{
		if (future_state.at(i) == stop_state)
		{
			switch (i)
			{
			case 0:
				JudgeDestinationMoveState(future_state.at(i));
				break;

			case 1:
				JudgeDestinationActionState(future_state.at(i));
				break;

			case 2:
				JudgeDestinationWeaponActionState(future_state.at(i));
				break;

			case 3:
				JudgeDestinationSpecialState(future_state.at(i));
				break;
			}

			return;
		}
	}
}

void PlayerStateController::JudgeDestinationMoveState(std::shared_ptr<IState<Player>>& stop_state)
{
	switch (static_cast<player_state::MoveStateKind>(stop_state->GetStateKind()))
	{
	case player_state::MoveStateKind::kMove:
		stop_state = m_states.at(typeid(player_state::MoveNull));
		break;

	default:
		break;
	}
}

void PlayerStateController::JudgeDestinationActionState(std::shared_ptr<IState<Player>>& stop_state)
{
	switch (static_cast<player_state::ActionStateKind>(stop_state->GetStateKind()))
	{
	case player_state::ActionStateKind::kCrouch:
		CommandHandler::GetInstance()->InitTriggerInputCount(CommandKind::kCrouch);
		stop_state = m_states.at(typeid(player_state::ActionNull));
		break;

	case player_state::ActionStateKind::kRun:
		CommandHandler::GetInstance()->InitTriggerInputCount(CommandKind::kRun);
		stop_state = m_states.at(typeid(player_state::ActionNull));
		break;

	default:
		break;
	}
}

void PlayerStateController::JudgeDestinationWeaponActionState(std::shared_ptr<IState<Player>>& stop_state)
{
	switch (static_cast<player_state::WeaponActionStateKind>(stop_state->GetStateKind()))
	{
	default:
		break;
	}
}

void PlayerStateController::JudgeDestinationSpecialState(std::shared_ptr<IState<Player>>& stop_state)
{
	switch (static_cast<player_state::SpecialStateKind>(stop_state->GetStateKind()))
	{
	default:
		break;
	}
}


#pragma region Try判定
bool PlayerStateController::TryMove()
{
	const auto command = CommandHandler::GetInstance();

	return((command->IsExecuting(CommandKind::kMoveUpPlayer)
		 || command->IsExecuting(CommandKind::kMoveDownPlayer)
		 || command->IsExecuting(CommandKind::kMoveLeftPlayer)
		 || command->IsExecuting(CommandKind::kMoveRightPlayer)));
}

bool PlayerStateController::TryRun()
{
	const auto command		= CommandHandler::GetInstance();
	const auto input		= InputChecker::GetInstance();
	const auto is_trigger	= command->GetInputModeKind(CommandKind::kRun) == InputModeKind::kTrigger ? true : false;

	// IDLEであった場合、ホールド方式に変更して判定を行う
	if (m_move_state.at(TimeKind::kCurrent)->GetStateKind() == static_cast<int>(player_state::MoveStateKind::kMoveNull))
	{
		command->SetInputMode(CommandKind::kRun, InputModeKind::kHold);
	}

	// 実行判定
	auto is_run = command->IsExecuting(CommandKind::kRun);

	// IDLEであるかつ現在ダッシュ状態である場合、ダッシュ状態を解除する
	if (   m_move_state  .at(TimeKind::kCurrent)->GetStateKind() == static_cast<int>(player_state::MoveStateKind::kMoveNull)
		&& m_action_state.at(TimeKind::kCurrent)->GetStateKind() == static_cast<int>(player_state::ActionStateKind::kRun))
	{
		is_run = false;
	}

	// もともとトリガー方式であれば元に戻す
	if (is_trigger) { command->SetInputMode(CommandKind::kRun, InputModeKind::kTrigger); }

	return is_run;
}

bool PlayerStateController::TryEquipGun(Player* player)
{
	return player->GetCurrentEquipWeaponKind() == WeaponKind::kGun && CommandHandler::GetInstance()->IsExecuting(CommandKind::kAimGun);
}

bool PlayerStateController::TryPullTrigger(Player* player)
{
	const auto gun				= std::dynamic_pointer_cast<GunBase>(player->GetCurrentHeldWeapon());
	const auto is_pull_trigger	= CommandHandler::GetInstance()->IsExecuting(CommandKind::kPullTrigger);
	
	if (gun)
	{
		if (is_pull_trigger)
		{
			gun->PullTrigger();
			return true;
		}
		else
		{
			gun->ReleaseTrigger();
		}
	}

	return false;
}

//bool PlayerStateController::TryAimGun(Player* obj)
//{
//	return obj->GetCurrentEquipWeaponKind() == WeaponKind::kGun && CommandHandler::GetInstance()->IsExecuting(CommandKind::kAimGun);
//}
#pragma endregion
	