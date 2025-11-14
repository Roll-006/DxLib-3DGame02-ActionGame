#include "player_state_controller.hpp"
#include "../Interface/i_melee_hittable.hpp"

PlayerStateController::PlayerStateController() : 
	m_change_weapon_after_state(nullptr)
{
	CreateState();
	AddCheckStopState();
	AddStopStatePair();

	// 初期ステート
	m_move_state		 [TimeKind::kPrev] = m_move_state		  [TimeKind::kCurrent] = GetState<player_state::Idle,				Player>();
	m_action_state		 [TimeKind::kPrev] = m_action_state		  [TimeKind::kCurrent] = GetState<player_state::ActionNull,			Player>();
	m_weapon_action_state[TimeKind::kPrev] = m_weapon_action_state[TimeKind::kCurrent] = GetState<player_state::WeaponActionNull,	Player>();
}

PlayerStateController::~PlayerStateController()
{
	m_move_state		 [TimeKind::kPrev] = m_move_state		  [TimeKind::kCurrent] = nullptr;
	m_action_state		 [TimeKind::kPrev] = m_action_state		  [TimeKind::kCurrent] = nullptr;
	m_weapon_action_state[TimeKind::kPrev] = m_weapon_action_state[TimeKind::kCurrent] = nullptr;
}

void PlayerStateController::Init(std::shared_ptr<Player> player)
{
	m_move_state			.at(TimeKind::kCurrent)->Exit(player);
	m_action_state			.at(TimeKind::kCurrent)->Exit(player);
	m_weapon_action_state	.at(TimeKind::kCurrent)->Exit(player);
}

void PlayerStateController::Update(std::shared_ptr<Player> player)
{
	ChangeState(player);

	m_move_state			.at(TimeKind::kCurrent)->Update(player);
	m_action_state			.at(TimeKind::kCurrent)->Update(player);
	m_weapon_action_state	.at(TimeKind::kCurrent)->Update(player);
}

void PlayerStateController::LateUpdate(std::shared_ptr<Player> player)
{
	m_move_state			.at(TimeKind::kCurrent)->LateUpdate(player);
	m_action_state			.at(TimeKind::kCurrent)->LateUpdate(player);
	m_weapon_action_state	.at(TimeKind::kCurrent)->LateUpdate(player);
}

void PlayerStateController::CreateState()
{
	m_states[typeid(player_state::Idle)]				 = std::make_shared<player_state::Idle>();
	m_states[typeid(player_state::Move)]				 = std::make_shared<player_state::Move>();

	m_states[typeid(player_state::ActionNull)]			 = std::make_shared<player_state::ActionNull>();
	m_states[typeid(player_state::Crouch)]				 = std::make_shared<player_state::Crouch>();
	m_states[typeid(player_state::Run)]					 = std::make_shared<player_state::Run>();
	m_states[typeid(player_state::TurnAround)]			 = std::make_shared<player_state::TurnAround>();
	m_states[typeid(player_state::CrouchTurnAround)]	 = std::make_shared<player_state::CrouchTurnAround>();
	m_states[typeid(player_state::Dead)]				 = std::make_shared<player_state::Dead>();
	m_states[typeid(player_state::Grabbed)]				 = std::make_shared<player_state::Grabbed>();
	m_states[typeid(player_state::RoundhouseKick)]		 = std::make_shared<player_state::RoundhouseKick>();
	m_states[typeid(player_state::FrontKick)]			 = std::make_shared<player_state::FrontKick>();
	m_states[typeid(player_state::Suplex)]				 = std::make_shared<player_state::Suplex>();
	m_states[typeid(player_state::Escape)]				 = std::make_shared<player_state::Escape>();
	m_states[typeid(player_state::StealthKill)]			 = std::make_shared<player_state::StealthKill>();
	m_states[typeid(player_state::FinishOff)]			 = std::make_shared<player_state::FinishOff>();
	m_states[typeid(player_state::EscapeWithKnife)]		 = std::make_shared<player_state::EscapeWithKnife>();
	m_states[typeid(player_state::VictoryPose)]			 = std::make_shared<player_state::VictoryPose>();

	m_states[typeid(player_state::WeaponActionNull)]	 = std::make_shared<player_state::WeaponActionNull>();
	m_states[typeid(player_state::EquipKnife)]			 = std::make_shared<player_state::EquipKnife>();
	m_states[typeid(player_state::AimKnife)]			 = std::make_shared<player_state::AimKnife>();
	m_states[typeid(player_state::StabKnife)]			 = std::make_shared<player_state::StabKnife>();
	m_states[typeid(player_state::FirstSideSlashKnife)]	 = std::make_shared<player_state::FirstSideSlashKnife>();
	m_states[typeid(player_state::SecondSideSlashKnife)] = std::make_shared<player_state::SecondSideSlashKnife>();
	m_states[typeid(player_state::SpinningSlashKnife)]	 = std::make_shared<player_state::SpinningSlashKnife>();
	m_states[typeid(player_state::Parry)]				 = std::make_shared<player_state::Parry>();
	m_states[typeid(player_state::EquipGun)]			 = std::make_shared<player_state::EquipGun>();
	m_states[typeid(player_state::AimGun)]				 = std::make_shared<player_state::AimGun>();
	m_states[typeid(player_state::Shot)]				 = std::make_shared<player_state::Shot>();
	m_states[typeid(player_state::ShotRocketLauncher)]	 = std::make_shared<player_state::ShotRocketLauncher>();
	m_states[typeid(player_state::Reload)]				 = std::make_shared<player_state::Reload>();
}

void PlayerStateController::AddStopStatePair()
{
	m_states.at(typeid(player_state::Dead))					->AddStopState(m_states.at(typeid(player_state::Move))					->GetStateHandle());
	//m_states.at(typeid(player_state::StealthKill))			->AddStopState(m_states.at(typeid(player_state::Move))					->GetStateHandle());
	//m_states.at(typeid(player_state::StealthKill))			->AddStopState(m_states.at(typeid(player_state::FirstSideSlashKnife))					->GetStateHandle());
	//m_states.at(typeid(player_state::StealthKill))			->AddStopState(m_states.at(typeid(player_state::SecondSideSlashKnife))					->GetStateHandle());
	//m_states.at(typeid(player_state::StealthKill))			->AddStopState(m_states.at(typeid(player_state::SpinningSlashKnife))					->GetStateHandle());

	m_states.at(typeid(player_state::AimKnife))				->AddStopState(m_states.at(typeid(player_state::Crouch))				->GetStateHandle());
	m_states.at(typeid(player_state::AimKnife))				->AddStopState(m_states.at(typeid(player_state::Run))					->GetStateHandle());
	m_states.at(typeid(player_state::StabKnife))			->AddStopState(m_states.at(typeid(player_state::Move))					->GetStateHandle());
	m_states.at(typeid(player_state::StabKnife))			->AddStopState(m_states.at(typeid(player_state::Crouch))				->GetStateHandle());
	m_states.at(typeid(player_state::StabKnife))			->AddStopState(m_states.at(typeid(player_state::Run))					->GetStateHandle());
	m_states.at(typeid(player_state::FirstSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Move))					->GetStateHandle());
	m_states.at(typeid(player_state::FirstSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Crouch))				->GetStateHandle());
	m_states.at(typeid(player_state::FirstSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Run))					->GetStateHandle());
	m_states.at(typeid(player_state::SecondSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Move))					->GetStateHandle());
	m_states.at(typeid(player_state::SecondSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Crouch))				->GetStateHandle());
	m_states.at(typeid(player_state::SecondSideSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Run))					->GetStateHandle());
	m_states.at(typeid(player_state::SpinningSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Move))					->GetStateHandle());
	m_states.at(typeid(player_state::SpinningSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Crouch))				->GetStateHandle());
	m_states.at(typeid(player_state::SpinningSlashKnife))	->AddStopState(m_states.at(typeid(player_state::Run))					->GetStateHandle());
	m_states.at(typeid(player_state::Parry))				->AddStopState(m_states.at(typeid(player_state::Crouch))				->GetStateHandle());
	m_states.at(typeid(player_state::Parry))				->AddStopState(m_states.at(typeid(player_state::Run))					->GetStateHandle());
	m_states.at(typeid(player_state::AimGun))				->AddStopState(m_states.at(typeid(player_state::Crouch))				->GetStateHandle());
	m_states.at(typeid(player_state::AimGun))				->AddStopState(m_states.at(typeid(player_state::Run))					->GetStateHandle());
	m_states.at(typeid(player_state::Shot))					->AddStopState(m_states.at(typeid(player_state::Crouch))				->GetStateHandle());
	m_states.at(typeid(player_state::Shot))					->AddStopState(m_states.at(typeid(player_state::Run))					->GetStateHandle());
	m_states.at(typeid(player_state::ShotRocketLauncher))	->AddStopState(m_states.at(typeid(player_state::Move))					->GetStateHandle());
	m_states.at(typeid(player_state::ShotRocketLauncher))	->AddStopState(m_states.at(typeid(player_state::Crouch))				->GetStateHandle());
	m_states.at(typeid(player_state::ShotRocketLauncher))	->AddStopState(m_states.at(typeid(player_state::Run))					->GetStateHandle());
}

void PlayerStateController::AddCheckStopState()
{
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::Move))					->GetStateHandle());
	
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::Crouch))				->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::Run))					->GetStateHandle());

	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::EquipKnife))			->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::AimKnife))				->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::StabKnife))			->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::FirstSideSlashKnife))	->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::SecondSideSlashKnife))	->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::SpinningSlashKnife))	->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::Parry))				->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::EquipGun))				->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::AimGun))				->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::Shot))					->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::ShotRocketLauncher))	->GetStateHandle());
	m_check_stop_state_handles.emplace_back(m_states.at(typeid(player_state::Reload))				->GetStateHandle());
}

void PlayerStateController::ChangeState(std::shared_ptr<Player>& player)
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
}

std::vector<std::shared_ptr<IState<Player>>> PlayerStateController::CreateChangeState(std::shared_ptr<Player>& player)
{
	// 次変更予定のステート
	std::vector<std::shared_ptr<IState<Player>>> next_state
	{
		m_move_state		 .at(TimeKind::kCurrent)->ChangeState(player),
		m_action_state		 .at(TimeKind::kCurrent)->ChangeState(player),
		m_weapon_action_state.at(TimeKind::kCurrent)->ChangeState(player)
	};

	std::vector<int> check_stop_state_index;
	auto future_state = CreateFutureState(next_state);

	// 上の階層にあるステートの停止処理
	for (int i = static_cast<int>(future_state.size() - 1); i >= 0; --i)
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
	return std::vector<std::shared_ptr<IState<Player>>>
	{
		future_state.at(0) == m_move_state			.at(TimeKind::kCurrent) ? nullptr : future_state.at(0),
		future_state.at(1) == m_action_state		.at(TimeKind::kCurrent) ? nullptr : future_state.at(1),
		future_state.at(2) == m_weapon_action_state	.at(TimeKind::kCurrent) ? nullptr : future_state.at(2)
	};
}

std::vector<std::shared_ptr<IState<Player>>> PlayerStateController::CreateFutureState(const std::vector<std::shared_ptr<IState<Player>>>& next_state)
{
	// 変更があったステートは置き換える
	return std::vector<std::shared_ptr<IState<Player>>>
	{
		next_state.at(0) == nullptr ? m_move_state			.at(TimeKind::kCurrent) : next_state.at(0),
		next_state.at(1) == nullptr ? m_action_state		.at(TimeKind::kCurrent) : next_state.at(1),
		next_state.at(2) == nullptr ? m_weapon_action_state	.at(TimeKind::kCurrent) : next_state.at(2)
	};
}

void PlayerStateController::StopState(std::vector<std::shared_ptr<IState<Player>>>& future_state, const std::shared_ptr<IState<Player>>& stop_state)
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
		stop_state = m_states.at(typeid(player_state::Idle));
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
		CommandHandler::GetInstance()->InitCurrentTriggerInputCount(CommandKind::kCrouch);
		stop_state = m_states.at(typeid(player_state::ActionNull));
		break;

	case player_state::ActionStateKind::kRun:
		CommandHandler::GetInstance()->InitCurrentTriggerInputCount(CommandKind::kRun);
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
	case player_state::WeaponActionStateKind::kEquipKnife:
	case player_state::WeaponActionStateKind::kAimKnife:
	case player_state::WeaponActionStateKind::kStabKnife:
	case player_state::WeaponActionStateKind::kFirstSideSlashKnife:
	case player_state::WeaponActionStateKind::kSecondSideSlashKnife:
	case player_state::WeaponActionStateKind::kSpinningSlashKnife:
	case player_state::WeaponActionStateKind::kParry:
	case player_state::WeaponActionStateKind::kEquipGun:
	case player_state::WeaponActionStateKind::kAimGun:
	case player_state::WeaponActionStateKind::kShot:
	case player_state::WeaponActionStateKind::kShotRocketLauncher:
	case player_state::WeaponActionStateKind::kReload:
		stop_state = m_states.at(typeid(player_state::WeaponActionNull));
		break;

	default:
		break;
	}
}


#pragma region Try判定
bool PlayerStateController::TryMove(std::shared_ptr<Player>& player)
{
	if (!player->CanControl()) { return false; }

	const auto command = CommandHandler::GetInstance();

	return((command->IsExecute(CommandKind::kMoveUpPlayer,		TimeKind::kCurrent)
		 || command->IsExecute(CommandKind::kMoveDownPlayer,	TimeKind::kCurrent)
		 || command->IsExecute(CommandKind::kMoveLeftPlayer,	TimeKind::kCurrent)
		 || command->IsExecute(CommandKind::kMoveRightPlayer,	TimeKind::kCurrent)));
}

bool PlayerStateController::TryRun(std::shared_ptr<Player>& player)
{
	if (!player->CanControl()) { return false; }

	const auto command			= CommandHandler::GetInstance();
	const auto input			= InputChecker::GetInstance();
	const auto move_state_kind	= static_cast<player_state::MoveStateKind>(m_move_state.at(TimeKind::kCurrent)->GetStateKind());

	if (move_state_kind == player_state::MoveStateKind::kMove)
	{
		if (command->IsExecute(CommandKind::kRun, TimeKind::kCurrent))
		{
			return true;
		}
	}
	// IDLEであった場合、次フレームのためにトリガーカウントを増やしておく
	else
	{
		// 現在の入力方式を保存する
		const bool is_trigger = command->GetInputModeKind(CommandKind::kRun) == InputModeKind::kTrigger ? true : false;

		// 入力モードを強制的にホールド方式に変更
		command->SetInputMode(CommandKind::kRun, InputModeKind::kHold);

		// もともとトリガー方式であれば元に戻す
		if (is_trigger) { command->SetInputMode(CommandKind::kRun, InputModeKind::kTrigger); }
	}

	return false;
}

bool PlayerStateController::TryDead(std::shared_ptr<Player>& player)
{
	return !player->IsAlive();
}

bool PlayerStateController::TryGrabbed(std::shared_ptr<Player>& player)
{
	return player->IsGrabbed();
}

bool PlayerStateController::TryFrontKick(std::shared_ptr<Player>& player)
{
	if (!player->CanControl())																{ return false; }
	if (!player->GetMeleeTarget())															{ return false; }
	if (!CommandHandler::GetInstance()->IsExecute(CommandKind::kMelee, TimeKind::kCurrent)) { return false; }

	// ターゲットをメレー対象となるインターフェイスに変更
	const auto melee_target		= player->GetMeleeTarget();
	const auto target_obj		= std::dynamic_pointer_cast<ObjBase>(player->GetMeleeTarget());

	// 怯み中(しゃがみ)でない場合は遷移を許可しない
	if (!melee_target->IsCrouchStun()) { return false; }

	// プレイヤーがターゲットの前方にいない場合は遷移を許可しない
	const auto target_forward = target_obj->GetTransform()->GetForward(CoordinateKind::kWorld);
	auto dir = player->GetTransform()->GetPos(CoordinateKind::kWorld) - target_obj->GetTransform()->GetPos(CoordinateKind::kWorld);
	dir.y = 0.0f;
	dir = v3d::GetNormalizedV(dir);
	if (math::GetAngleBetweenTwoVector(target_forward, dir) > 45.0f * math::kDegToRad) { return false; }

	// プレイヤーの入力に即座に対応できるようにmove_dirを基準になす角を取得
	const auto player_move_dir	= player->GetCurrentMoveDir();
	const auto player_dir		= player_move_dir != v3d::GetZeroV() ? player_move_dir : player->GetCurrentLookDir();
	const auto forward_angle	= math::GetAngleBetweenTwoVector(target_forward, player_dir);

	// 正面から蹴った場合にステートを遷移
	return forward_angle >= 135.0f * math::kDegToRad;
}

bool PlayerStateController::TryRoundhouseKick(std::shared_ptr<Player>& player)
{
	if (!player->CanControl())																{ return false; }
	if (!player->GetMeleeTarget())															{ return false; }
	if (!CommandHandler::GetInstance()->IsExecute(CommandKind::kMelee, TimeKind::kCurrent)) { return false; }

	// ターゲットをメレー対象となるインターフェイスに変更
	const auto melee_target = player->GetMeleeTarget();
	const auto target_obj	= std::dynamic_pointer_cast<ObjBase>(player->GetMeleeTarget());
	
	return melee_target->IsStandStun() || melee_target->IsCrouchStun();
}

bool PlayerStateController::TryStealthKill(std::shared_ptr<Player>& player)
{
	if (!player->CanControl())																	{ return false; }
	if (!player->GetStealthKillTarget())														{ return false; }
	if (!CommandHandler::GetInstance()->IsExecute(CommandKind::kAttack, TimeKind::kCurrent))	{ return false; }

	return true;
}

bool PlayerStateController::TryEquipKnifeShortcut(std::shared_ptr<Player>& player)
{
	const auto weapon = player->GetCurrentHeldWeapon();

	if (weapon)
	{
		return weapon->GetWeaponKind() == WeaponKind::kKnife;
	}
	return false;
}

bool PlayerStateController::TryAimKnife(std::shared_ptr<Player>& player)
{
	if (!player->CanControl())	{ return false; }
	if (!player->IsAlive())		{ return false; }

	const auto command = CommandHandler::GetInstance();
	return command->IsExecute(CommandKind::kAimKnife, TimeKind::kCurrent) && player->GetCurrentEquipWeapon(WeaponSlotKind::kSub);
}

bool PlayerStateController::TryFirstSideSlashKnife(std::shared_ptr<Player>& player)
{
	if (!player->CanControl())	{ return false; }
	if (!player->IsAlive())		{ return false; }

	const auto command = CommandHandler::GetInstance();
	return command->IsExecute(CommandKind::kAttack, TimeKind::kCurrent);
}

bool PlayerStateController::TrySpinningSlash(std::shared_ptr<Player>& player)
{
	if (!player->CanControl()) { return false; }

	const bool is_run = m_action_state.at(TimeKind::kCurrent)->GetStateKind() == static_cast<int>(player_state::ActionStateKind::kRun);

	return  is_run && CommandHandler::GetInstance()->IsExecute(CommandKind::kAttack, TimeKind::kCurrent);
}

bool PlayerStateController::TryEquipGun(std::shared_ptr<Player>& player)
{
	if (!player->CanControl()) { return false; }

	return player->GetCurrentEquipWeaponKind(WeaponSlotKind::kMain) == WeaponKind::kGun
		&& CommandHandler::GetInstance()->IsExecute(CommandKind::kAimGun, TimeKind::kCurrent);
}

bool PlayerStateController::TryEquipGunShortcut(std::shared_ptr<Player>& player)
{
	const auto weapon = player->GetCurrentHeldWeapon();

	if (weapon)
	{
		return weapon->GetWeaponKind() == WeaponKind::kGun;
	}
	return false;
}

bool PlayerStateController::TryPullTrigger(std::shared_ptr<Player>& player)
{
	if (!player->CanControl()) { return false; }

	const auto gun = std::dynamic_pointer_cast<GunBase>(player->GetCurrentHeldWeapon());

	if (gun)
	{
		if (CommandHandler::GetInstance()->IsExecute(CommandKind::kPullTrigger, TimeKind::kCurrent))
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

bool PlayerStateController::TryReload(std::shared_ptr<Player>& player)
{
	if (!player->CanControl())	{ return false; }
	if (!player->IsAlive())		{ return false; }

	const auto gun			= std::dynamic_pointer_cast<GunBase>(player->GetCurrentEquipWeapon(WeaponSlotKind::kMain));
	const auto ammo_holder	= player->GetAmmoHolder();

	if (gun)
	{
		switch (gun->GetGunKind())
		{
		case GunKind::kSubmachineGun:
			if (ammo_holder->GetCurrentAmmoNum(typeid(AssaultRifleAmmoBox)) <= 0) { return false; }
			break;

		case GunKind::kRocketLauncher:
			if (ammo_holder->GetCurrentAmmoNum(typeid(RocketBombBox))		<= 0) { return false; }
			break;

		default:
			break;
		}

		return gun->CanReload() && CommandHandler::GetInstance()->IsExecute(CommandKind::kReload, TimeKind::kCurrent);
	}

	return false;
}

bool PlayerStateController::TryPullTriggerReload(std::shared_ptr<Player>& player)
{
	if (!player->CanControl()) { return false; }

	const auto gun = std::dynamic_pointer_cast<GunBase>(player->GetCurrentHeldWeapon());
	if (!gun) { return false; }
	if (gun->GetCurrentRemainingBulletNum() > 0) { return false; }

	const auto command		= CommandHandler::GetInstance();
	const auto ammo_holder	= player->GetAmmoHolder();
	if (gun)
	{
		switch (gun->GetGunKind())
		{
		case GunKind::kSubmachineGun:
			if (ammo_holder->GetCurrentAmmoNum(typeid(AssaultRifleAmmoBox)) <= 0) { return false; }
			break;

		case GunKind::kRocketLauncher:
			if (ammo_holder->GetCurrentAmmoNum(typeid(RocketBombBox))		<= 0) { return false; }
			break;

		default:
			break;
		}

		return !command->IsExecute(CommandKind::kPullTrigger, TimeKind::kPrev) && command->IsExecute(CommandKind::kPullTrigger, TimeKind::kCurrent);
	}

	return false;
}
#pragma endregion
