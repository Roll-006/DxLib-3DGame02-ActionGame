#pragma once
#include "../Base/one_instance_singleton_base.hpp"

#include "../State/PlayerState/move_null.hpp"
#include "../State/PlayerState/move.hpp"

#include "../State/PlayerState/action_null.hpp"
#include "../State/PlayerState/squat.hpp"
#include "../State/PlayerState/run.hpp"
#include "../State/PlayerState/turn_around.hpp"

#include "../State/PlayerState/weapon_action_null.hpp"
#include "../State/PlayerState/knife_equipped.hpp"
#include "../State/PlayerState/stab_knife.hpp"
#include "../State/PlayerState/side_slash_knife.hpp"
#include "../State/PlayerState/parry.hpp"
#include "../State/PlayerState/gun_equipped.hpp"
#include "../State/PlayerState/aiming.hpp"
#include "../State/PlayerState/shot.hpp"
#include "../State/PlayerState/reload.hpp"

#include "../State/PlayerState/special_null.hpp"
#include "../State/PlayerState/roundhouse_kick.hpp"
#include "../State/PlayerState/front_kick.hpp"
#include "../State/PlayerState/suplex.hpp"
#include "../State/PlayerState/escape.hpp"
#include "../State/PlayerState/stealth_kill.hpp"
#include "../State/PlayerState/finish_off.hpp"
#include "../State/PlayerState/escape_with_knife.hpp"

class PlayerStateManager final : public OneInstanceSingletonBase<PlayerStateManager>
{
public:
	PlayerStateManager();
	~PlayerStateManager();

	void Update		(const Player* player);
	void ChangeState(const Player* player);

private:
	std::shared_ptr<IMoveState<Player>>						m_current_move_state;				// ˆÚ“®ó‘Ô
	std::shared_ptr<IActionState<Player>>					m_current_action_state;				// s“®ó‘Ô
	std::shared_ptr<IWeaponActionState<Player>>				m_current_weapon_action_state;		// •Ší‚ÉŠÖ‚·‚és“®ó‘Ô
	std::shared_ptr<ISpecialState<Player>>					m_current_special_state;			// “Áês“®ó‘Ô

	std::shared_ptr<player_state::MoveNull>					m_idle;
	std::shared_ptr<player_state::Move>						m_move;

	std::shared_ptr<player_state::ActionNull>				m_action_null;
	std::shared_ptr<player_state::Squat>					m_squat;
	std::shared_ptr<player_state::Run>						m_run;
	std::shared_ptr<player_state::TurnAround>				m_turn_around;

	std::shared_ptr<player_state::WeaponActionNull>			m_weapon_action_null;
	std::shared_ptr<player_state::KnifeEquipped>			m_knife_equipped;
	std::shared_ptr<player_state::StabKnife>				m_stab_knife;
	std::shared_ptr<player_state::SideSlashKnife>			m_side_slash_knife;
	std::shared_ptr<player_state::Parry>					m_parry;
	std::shared_ptr<player_state::GunEquipped>				m_gun_equipped;
	std::shared_ptr<player_state::Aiming>					m_aiming;
	std::shared_ptr<player_state::Shot>						m_shot;
	std::shared_ptr<player_state::Reload>					m_reload;

	std::shared_ptr<player_state::SpecialNull>				m_special_null;
	std::shared_ptr<player_state::RoundhouseKick>			m_roundhouse_kick;
	std::shared_ptr<player_state::FrontKick>				m_front_kick;
	std::shared_ptr<player_state::Suplex>					m_suplex;
	std::shared_ptr<player_state::Escape>					m_escape;
	std::shared_ptr<player_state::StealthKill>				m_stealth_kill;
	std::shared_ptr<player_state::FinishOff>				m_finish_off;
	std::shared_ptr<player_state::EscapeWithKnife>			m_escape_with_knife;
};
