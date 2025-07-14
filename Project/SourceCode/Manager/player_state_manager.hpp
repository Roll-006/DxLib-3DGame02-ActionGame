#pragma once
#include "../Base/one_instance_singleton_base.hpp"

#include "../State/idle_state.hpp"
#include "../State/move_state.hpp"

#include "../State/extension_move_null_state.hpp"
#include "../State/squat_state.hpp"
#include "../State/run_state.hpp"

#include "../State/attack_null_state.hpp"
#include "../State/roundhouse_kick_state.hpp"
#include "../State/front_kick_state.hpp"
#include "../State/suplex_state.hpp"
#include "../State/escape_state.hpp"
#include "../State/knife_equipped_state.hpp"
#include "../State/stab_knife_state.hpp"
#include "../State/side_slash_knife_state.hpp"
#include "../State/parry_state.hpp"
#include "../State/stealth_kill_state.hpp"
#include "../State/finish_off_state.hpp"
#include "../State/escape_with_knife_state.hpp"
#include "../State/gun_equipped_state.hpp"
#include "../State/aiming_state.hpp"
#include "../State/shot_state.hpp"
#include "../State/reload_state.hpp"

class PlayerStateManager final : public OneInstanceSingletonBase<PlayerStateManager>
{
public:
	PlayerStateManager();
	~PlayerStateManager();

	void Update		(const Player* player);
	void ChangeState(const Player* player);

private:
	std::shared_ptr<IState<Player>>				m_current_basis_move_state;			// Šî‘b“I‚ÈˆÚ“®ó‘Ô
	std::shared_ptr<IState<Player>>				m_current_extension_move_state;		// ’Ç‰Á‚ÌˆÚ“®ó‘Ô
	std::shared_ptr<IState<Player>>				m_current_attack_state;				// UŒ‚‚ÉŠÖ‚·‚éó‘Ô

	std::shared_ptr<IdleState>					m_idle_state;
	std::shared_ptr<MoveState>					m_move_state;

	std::shared_ptr<ExtensionMoveNullState>		m_extension_move_null_state;
	std::shared_ptr<SquatState>					m_squat_state;
	std::shared_ptr<RunState>					m_run_state;

	std::shared_ptr<AttackNullState>			m_attack_null_state;
	std::shared_ptr<RoundhouseKickState>		m_roundhouse_kick_state;
	std::shared_ptr<FrontKickState>				m_front_kick_state;
	std::shared_ptr<SuplexState>				m_suplex_state;
	std::shared_ptr<EscapeState>				m_escape_state;
	std::shared_ptr<KnifeEquippedState>			m_knife_equipped_state;
	std::shared_ptr<StabKnifeState>				m_stab_knife_state;
	std::shared_ptr<SideSlashKnifeState>		m_side_slash_knife_state;
	std::shared_ptr<ParryState>					m_parry_state;
	std::shared_ptr<StealthKillState>			m_stealth_kill_state;
	std::shared_ptr<FinishOffState>				m_finish_off_state;
	std::shared_ptr<GunEquippedState>			m_gun_equipped_state;
	std::shared_ptr<AimingState>				m_aiming_state;
	std::shared_ptr<ShotState>					m_shot_state;
	std::shared_ptr<ReloadState>				m_reload_state;
};
