#pragma once
#include "../Base/one_instance_singleton_base.hpp"

#include "../State/PlayerState/idle.hpp"]
#include "../State/PlayerState/move.hpp"

#include "../State/PlayerState/extension_move_null.hpp"
#include "../State/PlayerState/squat.hpp"
#include "../State/PlayerState/run.hpp"

#include "../State/PlayerState/attack_null.hpp"
#include "../State/PlayerState/roundhouse_kick.hpp"
#include "../State/PlayerState/front_kick.hpp"
#include "../State/PlayerState/suplex.hpp"
#include "../State/PlayerState/escape.hpp"
#include "../State/PlayerState/knife_equipped.hpp"
#include "../State/PlayerState/stab_knife.hpp"
#include "../State/PlayerState/side_slash_knife.hpp"
#include "../State/PlayerState/parry.hpp"
#include "../State/PlayerState/stealth_kill.hpp"
#include "../State/PlayerState/finish_off.hpp"
#include "../State/PlayerState/escape_with_knife.hpp"
#include "../State/PlayerState/gun_equipped.hpp"
#include "../State/PlayerState/aiming.hpp"
#include "../State/PlayerState/shot.hpp"
#include "../State/PlayerState/reload.hpp"

class PlayerStateManager final : public OneInstanceSingletonBase<PlayerStateManager>
{
public:
	PlayerStateManager();
	~PlayerStateManager();

	void Update		(const Player* player);
	void ChangeState(const Player* player);

private:
	std::shared_ptr<IState<Player>>						m_current_basis_move_state;			// Šî‘b“I‚ÈˆÚ“®ó‘Ô
	std::shared_ptr<IState<Player>>						m_current_extension_move_state;		// ’Ç‰Á‚ÌˆÚ“®ó‘Ô
	std::shared_ptr<IState<Player>>						m_current_attack_state;				// UŒ‚‚ÉŠÖ‚·‚éó‘Ô

	std::shared_ptr<player_state::Idle>					m_idle_state;
	std::shared_ptr<player_state::Move>					m_move_state;

	std::shared_ptr<player_state::ExtensionMoveNull>	m_extension_move_null_state;
	std::shared_ptr<player_state::Squat>				m_squat_state;
	std::shared_ptr<player_state::Run>					m_run_state;

	std::shared_ptr<player_state::AttackNull>			m_attack_null_state;
	std::shared_ptr<player_state::RoundhouseKick>		m_roundhouse_kick_state;
	std::shared_ptr<player_state::FrontKick>			m_front_kick_state;
	std::shared_ptr<player_state::Suplex>				m_suplex_state;
	std::shared_ptr<player_state::Escape>				m_escape_state;
	std::shared_ptr<player_state::KnifeEquipped>		m_knife_equipped_state;
	std::shared_ptr<player_state::StabKnife>			m_stab_knife_state;
	std::shared_ptr<player_state::SideSlashKnife>		m_side_slash_knife_state;
	std::shared_ptr<player_state::Parry>				m_parry_state;
	std::shared_ptr<player_state::StealthKill>			m_stealth_kill_state;
	std::shared_ptr<player_state::FinishOff>			m_finish_off_state;
	std::shared_ptr<player_state::GunEquipped>			m_gun_equipped_state;
	std::shared_ptr<player_state::Aiming>				m_aiming_state;
	std::shared_ptr<player_state::Shot>					m_shot_state;
	std::shared_ptr<player_state::Reload>				m_reload_state;
};
