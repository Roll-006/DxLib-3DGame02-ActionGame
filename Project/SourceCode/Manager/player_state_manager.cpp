#include "player_state_manager.hpp"

PlayerStateManager::PlayerStateManager() : 
	m_current_move_state			(nullptr),
	m_current_action_state			(nullptr),
	m_current_weapon_action_state	(nullptr),
	m_current_special_state			(nullptr),
	m_idle							(std::make_shared<player_state::MoveNull>()),
	m_move							(std::make_shared<player_state::Move>()),	
	m_action_null					(std::make_shared<player_state::ActionNull>()),
	m_squat							(std::make_shared<player_state::Squat>()),
	m_run							(std::make_shared<player_state::Run>()),	
	m_turn_around					(std::make_shared<player_state::TurnAround>()),
	m_weapon_action_null			(std::make_shared<player_state::WeaponActionNull>()),
	m_knife_equipped				(std::make_shared<player_state::KnifeEquipped>()),
	m_stab_knife					(std::make_shared<player_state::StabKnife>()),
	m_side_slash_knife				(std::make_shared<player_state::SideSlashKnife>()),
	m_parry							(std::make_shared<player_state::Parry>()),
	m_gun_equipped					(std::make_shared<player_state::GunEquipped>()),
	m_aiming						(std::make_shared<player_state::Aiming>()),
	m_shot							(std::make_shared<player_state::Shot>()),
	m_reload						(std::make_shared<player_state::Reload>()),
	m_special_null					(std::make_shared<player_state::SpecialNull>()),
	m_roundhouse_kick				(std::make_shared<player_state::RoundhouseKick>()),
	m_front_kick					(std::make_shared<player_state::FrontKick>()),
	m_suplex						(std::make_shared<player_state::Suplex>()),
	m_escape						(std::make_shared<player_state::Escape>()),
	m_stealth_kill					(std::make_shared<player_state::StealthKill>()),
	m_finish_off					(std::make_shared<player_state::FinishOff>()),
	m_escape_with_knife				(std::make_shared<player_state::EscapeWithKnife>())
{
	m_current_move_state			= m_idle;
	m_current_action_state			= m_action_null;
	m_current_weapon_action_state	= m_weapon_action_null;
	m_current_special_state			= m_special_null;
}

PlayerStateManager::~PlayerStateManager()
{

}

void PlayerStateManager::Update(const Player* player)
{
	m_current_move_state			->Update(player);
	m_current_action_state			->Update(player);
	m_current_weapon_action_state	->Update(player);
	m_current_special_state			->Update(player);
}

void PlayerStateManager::ChangeState(const Player* player)
{
	m_current_move_state			->ChangeState(player);
	m_current_action_state			->ChangeState(player);
	m_current_weapon_action_state	->ChangeState(player);
	m_current_special_state			->ChangeState(player);
}
