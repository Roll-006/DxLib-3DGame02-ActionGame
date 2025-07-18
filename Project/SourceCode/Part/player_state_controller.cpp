#include "player_state_controller.hpp"

PlayerStateController::PlayerStateController()
{
	m_states[typeid(player_state::MoveNull)]			= std::make_shared<player_state::MoveNull>();
	m_states[typeid(player_state::Move)]				= std::make_shared<player_state::Move>();
	m_states[typeid(player_state::ActionNull)]			= std::make_shared<player_state::ActionNull>();
	m_states[typeid(player_state::Crouch)]				= std::make_shared<player_state::Crouch>();
	m_states[typeid(player_state::Run)]					= std::make_shared<player_state::Run>();
	m_states[typeid(player_state::TurnAround)]			= std::make_shared<player_state::TurnAround>();
	m_states[typeid(player_state::CrouchTurnAround)]	= std::make_shared<player_state::CrouchTurnAround>();
	m_states[typeid(player_state::WeaponActionNull)]	= std::make_shared<player_state::WeaponActionNull>();
	m_states[typeid(player_state::KnifeEquipped)]		= std::make_shared<player_state::KnifeEquipped>();
	m_states[typeid(player_state::AimKnife)]			= std::make_shared<player_state::AimKnife>();
	m_states[typeid(player_state::StabKnife)]			= std::make_shared<player_state::StabKnife>();
	m_states[typeid(player_state::SideSlashKnife)]		= std::make_shared<player_state::SideSlashKnife>();
	m_states[typeid(player_state::Parry)]				= std::make_shared<player_state::Parry>();
	m_states[typeid(player_state::GunEquipped)]			= std::make_shared<player_state::GunEquipped>();
	m_states[typeid(player_state::AimGun)]				= std::make_shared<player_state::AimGun>();
	m_states[typeid(player_state::Shot)]				= std::make_shared<player_state::Shot>();
	m_states[typeid(player_state::Reload)]				= std::make_shared<player_state::Reload>();
	m_states[typeid(player_state::SpecialNull)]			= std::make_shared<player_state::SpecialNull>();
	m_states[typeid(player_state::RoundhouseKick)]		= std::make_shared<player_state::RoundhouseKick>();
	m_states[typeid(player_state::FrontKick)]			= std::make_shared<player_state::FrontKick>();
	m_states[typeid(player_state::Suplex)]				= std::make_shared<player_state::Suplex>();
	m_states[typeid(player_state::Escape)]				= std::make_shared<player_state::Escape>();
	m_states[typeid(player_state::StealthKill)]			= std::make_shared<player_state::StealthKill>();
	m_states[typeid(player_state::FinishOff)]			= std::make_shared<player_state::FinishOff>();
	m_states[typeid(player_state::EscapeWithKnife)]		= std::make_shared<player_state::EscapeWithKnife>();

	m_move_state			[TimeKind::kPrev] = m_move_state			[TimeKind::kCurrent] = GetState<player_state::MoveNull,			Player>();
	m_action_state			[TimeKind::kPrev] = m_action_state			[TimeKind::kCurrent] = GetState<player_state::ActionNull,		Player>();
	m_weapon_action_state	[TimeKind::kPrev] = m_weapon_action_state	[TimeKind::kCurrent] = GetState<player_state::WeaponActionNull,	Player>();
	m_special_state			[TimeKind::kPrev] = m_special_state			[TimeKind::kCurrent] = GetState<player_state::SpecialNull,		Player>();
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

void PlayerStateController::ChangeState(Player* player)
{
	std::shared_ptr<IState<Player>> move_state = m_move_state.at(TimeKind::kCurrent)->ChangeState(player);
	if (move_state)
	{
		m_move_state.at(TimeKind::kPrev)	= m_move_state.at(TimeKind::kCurrent);
		m_move_state.at(TimeKind::kCurrent)	= std::static_pointer_cast<MoveStateBase<Player>>(move_state);
		m_move_state.at(TimeKind::kPrev)	->Exit(player);
		m_move_state.at(TimeKind::kCurrent)	->Enter(player);
	}

	std::shared_ptr<IState<Player>> action_state = m_action_state.at(TimeKind::kCurrent)->ChangeState(player);
	if (action_state)
	{
		m_action_state.at(TimeKind::kPrev)	  = m_action_state.at(TimeKind::kCurrent);
		m_action_state.at(TimeKind::kCurrent) = std::static_pointer_cast<ActionStateBase<Player>>(action_state);
		m_action_state.at(TimeKind::kPrev)	  ->Exit (player);
		m_action_state.at(TimeKind::kCurrent) ->Enter(player);
	}

	std::shared_ptr<IState<Player>> weapon_action_state = m_weapon_action_state.at(TimeKind::kCurrent)->ChangeState(player);
	if (weapon_action_state)
	{
		m_weapon_action_state.at(TimeKind::kPrev)	 = m_weapon_action_state.at(TimeKind::kCurrent);
		m_weapon_action_state.at(TimeKind::kCurrent) = std::static_pointer_cast<WeaponActionStateBase<Player>>(weapon_action_state);
		m_weapon_action_state.at(TimeKind::kPrev)	 ->Exit (player);
		m_weapon_action_state.at(TimeKind::kCurrent) ->Enter(player);
	}

	std::shared_ptr<IState<Player>> special_state = m_special_state.at(TimeKind::kCurrent)->ChangeState(player);
	if (special_state)
	{
		m_special_state.at(TimeKind::kPrev)	   = m_special_state.at(TimeKind::kCurrent);
		m_special_state.at(TimeKind::kCurrent) = std::static_pointer_cast<SpecialStateBase<Player>>(special_state);
		m_special_state.at(TimeKind::kPrev)	   ->Exit (player);
		m_special_state.at(TimeKind::kCurrent) ->Enter(player);
	}

	//m_current_action_state			->ChangeState(player);
	//m_current_weapon_action_state	->ChangeState(player);
	//m_current_special_state			->ChangeState(player);
}


#pragma region Try”»’è
bool PlayerStateController::TryMoveForward()
{
	return CommandHandler::GetInstance()->IsExecutingCommand(CommandKind::kMoveUpPlayer);
}

bool PlayerStateController::TryMoveBackward()
{
	return CommandHandler::GetInstance()->IsExecutingCommand(CommandKind::kMoveDownPlayer);
}

bool PlayerStateController::TryMoveLeft()
{
	return CommandHandler::GetInstance()->IsExecutingCommand(CommandKind::kMoveLeftPlayer);
}

bool PlayerStateController::TryMoveRight()
{
	return CommandHandler::GetInstance()->IsExecutingCommand(CommandKind::kMoveRightPlayer);
}

bool PlayerStateController::TryMove()
{
	const auto command = CommandHandler::GetInstance();

	if ((	command->IsExecutingCommand(CommandKind::kMoveUpPlayer)
		||	command->IsExecutingCommand(CommandKind::kMoveDownPlayer)
		||	command->IsExecutingCommand(CommandKind::kMoveLeftPlayer)
		||	command->IsExecutingCommand(CommandKind::kMoveRightPlayer)))
	{
		return true;
	}

	return false;
}

bool PlayerStateController::TryRun()
{
	if (m_move_state.at(TimeKind::kCurrent)->GetStateKind() == static_cast<int>(player_state::MoveStateKind::kMove))
	{
		return CommandHandler::GetInstance()->IsExecutingCommand(CommandKind::kRun);
	}
	return false;
}

bool PlayerStateController::TryCrouch()
{
	return CommandHandler::GetInstance()->IsExecutingCommand(CommandKind::kCrouch);
}
#pragma endregion
