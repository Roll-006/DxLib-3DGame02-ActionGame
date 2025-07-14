#include "player_state_manager.hpp"

PlayerStateManager::PlayerStateManager() : 
	m_current_basis_move_state		(nullptr),
	m_current_extension_move_state	(nullptr),
	m_current_attack_state			(nullptr),
	m_idle_state					(std::make_shared<IdleState>()),
	m_move_state					(std::make_shared<MoveState>()),	
	m_extension_move_null_state		(std::make_shared<ExtensionMoveNullState>()),
	m_squat_state					(std::make_shared<SquatState>()),
	m_run_state						(std::make_shared<RunState>()),	
	m_attack_null_state				(std::make_shared<AttackNullState>()),
	m_roundhouse_kick_state			(std::make_shared<RoundhouseKickState>()),
	m_front_kick_state				(std::make_shared<FrontKickState>()),
	m_suplex_state					(std::make_shared<SuplexState>()),
	m_escape_state					(std::make_shared<EscapeState>()),
	m_knife_equipped_state			(std::make_shared<KnifeEquippedState>()),
	m_stab_knife_state				(std::make_shared<StabKnifeState>()),
	m_side_slash_knife_state		(std::make_shared<SideSlashKnifeState>()),
	m_parry_state					(std::make_shared<ParryState>()),
	m_stealth_kill_state			(std::make_shared<StealthKillState>()),
	m_finish_off_state				(std::make_shared<FinishOffState>()),
	m_gun_equipped_state			(std::make_shared<GunEquippedState>()),
	m_aiming_state					(std::make_shared<AimingState>()),
	m_shot_state					(std::make_shared<ShotState>()),
	m_reload_state					(std::make_shared<ReloadState>())
{
	m_current_basis_move_state		= m_idle_state;
	m_current_extension_move_state	= m_extension_move_null_state;
	m_current_attack_state			= m_attack_null_state;
}

PlayerStateManager::~PlayerStateManager()
{

}

void PlayerStateManager::Update(const Player* player)
{
	m_current_basis_move_state		->Update(player);
	m_current_extension_move_state	->Update(player);
	m_current_attack_state			->Update(player);
}

void PlayerStateManager::ChangeState(const Player* player)
{
	m_current_basis_move_state		->ChangeState(player);
	m_current_extension_move_state	->ChangeState(player);
	m_current_attack_state			->ChangeState(player);
}
