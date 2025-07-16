#include "player_state_controller.hpp"

PlayerStateController::PlayerStateController() :
	m_current_move_state			(nullptr),
	m_current_action_state			(nullptr),
	m_current_weapon_action_state	(nullptr),
	m_current_special_state			(nullptr)
{
	m_states[typeid(player_state::MoveNull)]			= std::make_shared<player_state::MoveNull>();
	m_states[typeid(player_state::Move)]				= std::make_shared<player_state::Move>();
	m_states[typeid(player_state::ActionNull)]			= std::make_shared<player_state::ActionNull>();
	m_states[typeid(player_state::Crouch)]				= std::make_shared<player_state::Crouch>();
	m_states[typeid(player_state::Run)]					= std::make_shared<player_state::Run>();
	m_states[typeid(player_state::TurnAround)]			= std::make_shared<player_state::TurnAround>();
	m_states[typeid(player_state::WeaponActionNull)]	= std::make_shared<player_state::WeaponActionNull>();
	m_states[typeid(player_state::KnifeEquipped)]		= std::make_shared<player_state::KnifeEquipped>();
	m_states[typeid(player_state::StabKnife)]			= std::make_shared<player_state::StabKnife>();
	m_states[typeid(player_state::SideSlashKnife)]		= std::make_shared<player_state::SideSlashKnife>();
	m_states[typeid(player_state::Parry)]				= std::make_shared<player_state::Parry>();
	m_states[typeid(player_state::GunEquipped)]			= std::make_shared<player_state::GunEquipped>();
	m_states[typeid(player_state::Aiming)]				= std::make_shared<player_state::Aiming>();
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

	m_current_move_state			= GetState<player_state::MoveNull,			Player>();
	m_current_action_state			= GetState<player_state::ActionNull,		Player>();
	m_current_weapon_action_state	= GetState<player_state::WeaponActionNull,	Player>();
	m_current_special_state			= GetState<player_state::SpecialNull,		Player>();
}

PlayerStateController::~PlayerStateController()
{

}

void PlayerStateController::Update(Player* player)
{
	m_current_move_state			->Update(player);
	m_current_action_state			->Update(player);
	m_current_weapon_action_state	->Update(player);
	m_current_special_state			->Update(player);
}

void PlayerStateController::ChangeState(const Player* player)
{
	std::shared_ptr<IState<Player>> move_state = m_current_move_state->ChangeState(player);
	if (move_state)
	{
		m_current_move_state = nullptr;
		m_current_move_state = std::static_pointer_cast<MoveStateBase<Player>>(move_state);
		m_current_move_state->Enter(player);
	}

	//m_current_action_state			->ChangeState(player);
	//m_current_weapon_action_state	->ChangeState(player);
	//m_current_special_state			->ChangeState(player);
}


#pragma region Try判定
bool PlayerStateController::TryMove()
{
	const auto command = CommandHandler::GetInstance();

	if ((	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveUpPlayer)
		||	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveDownPlayer)
		||	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveLeftPlayer)
		||	command->GetCurrentFrameExecuteInputCode(CommandKind::kMoveRightPlayer)))
	{
		return true;
	}

	return false;
}

bool PlayerStateController::TryRun()
{
	const auto command	= CommandHandler::GetInstance();
	const auto input	= InputChecker	::GetInstance();
	const auto code		= *command->GetCurrentFrameExecuteInputCode(CommandKind::kRun);

	switch (command->GetInputModeKind(CommandHandler::MoveKind::kRun))
	{
	case InputModeKind::kTrigger:
		// トリガーの入力回数が偶数回の場合、Runへの移行を許可
		if (input->GetInputState(code) == InputState::kSingle)
		{
			command->CountUpTrigger(CommandHandler::MoveKind::kRun);

			return command->GetTriggerCount(CommandHandler::MoveKind::kRun) % 2 ? true : false;
		}
		break;

	case InputModeKind::kHold:
		// ホールド中の場合、Runへの移行を許可
		if (input->GetInputState(code) == InputState::kHold)
		{
			return true;
		}
		break;
	}

	return false;
}

bool PlayerStateController::TryCrouch()
{
	const auto command	= CommandHandler::GetInstance();
	const auto input	= InputChecker	::GetInstance();
	const auto code		= *command->GetCurrentFrameExecuteInputCode(CommandKind::kCrouch);

	switch (command->GetInputModeKind(CommandHandler::MoveKind::kCrouch))
	{
	case InputModeKind::kTrigger:
		// トリガーの入力回数が偶数回の場合、Crouchへの移行を許可
		if (input->GetInputState(code) == InputState::kSingle)
		{
			command->CountUpTrigger(CommandHandler::MoveKind::kCrouch);

			return command->GetTriggerCount(CommandHandler::MoveKind::kCrouch) % 2 ? true : false;
		}
		break;

	case InputModeKind::kHold:
		// ホールド中の場合、Crouchへの移行を許可
		if (input->GetInputState(code) == InputState::kHold)
		{
			return true;
		}
		break;
	}

	return false;
}
#pragma endregion
