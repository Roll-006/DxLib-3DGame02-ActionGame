#include "player_state_manager.hpp"

PlayerStateManager::PlayerStateManager() : 
	m_current_basis_move_state		(nullptr),
	m_current_extension_move_state	(nullptr),
	m_current_attack_state			(nullptr),
	m_idle_state					(std::make_shared<player_state::Idle>()),
	m_move_state					(std::make_shared<player_state::Move>()),	
	m_extension_move_null_state		(std::make_shared<player_state::ExtensionMoveNull>()),
	m_squat_state					(std::make_shared<player_state::Squat>()),
	m_run_state						(std::make_shared<player_state::Run>()),	
	m_attack_null_state				(std::make_shared<player_state::AttackNull>()),
	m_roundhouse_kick_state			(std::make_shared<player_state::RoundhouseKick>()),
	m_front_kick_state				(std::make_shared<player_state::FrontKick>()),
	m_suplex_state					(std::make_shared<player_state::Suplex>()),
	m_escape_state					(std::make_shared<player_state::Escape>()),
	m_knife_equipped_state			(std::make_shared<player_state::KnifeEquipped>()),
	m_stab_knife_state				(std::make_shared<player_state::StabKnife>()),
	m_side_slash_knife_state		(std::make_shared<player_state::SideSlashKnife>()),
	m_parry_state					(std::make_shared<player_state::Parry>()),
	m_stealth_kill_state			(std::make_shared<player_state::StealthKill>()),
	m_finish_off_state				(std::make_shared<player_state::FinishOff>()),
	m_gun_equipped_state			(std::make_shared<player_state::GunEquipped>()),
	m_aiming_state					(std::make_shared<player_state::Aiming>()),
	m_shot_state					(std::make_shared<player_state::Shot>()),
	m_reload_state					(std::make_shared<player_state::Reload>())
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
