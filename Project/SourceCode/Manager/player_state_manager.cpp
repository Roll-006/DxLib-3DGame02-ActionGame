#include "player_state_manager.hpp"

PlayerStateManager::PlayerStateManager() : 
	m_current_basis_move_state		(nullptr),
	m_current_extension_move_state	(nullptr),
	m_current_attack_state			(nullptr),
	m_idle_state					(std::make_shared<IdleState>()),
	m_move_state					(std::make_shared<MoveState>()),
	m_squat_state					(std::make_shared<SquatState>()),
	m_run_state						(std::make_shared<RunState>()),
	m_non_weapon_state				(std::make_shared<NonWeaponState>()),
	m_aiming_state					(std::make_shared<AimingState>()),
	m_shot_state					(std::make_shared<ShotState>()),
	m_reload_state					(std::make_shared<ReloadState>())
{
	m_current_basis_move_state	= m_idle_state;
	m_current_attack_state		= m_non_weapon_state;
}

PlayerStateManager::~PlayerStateManager()
{

}

void PlayerStateManager::Update()
{

}
