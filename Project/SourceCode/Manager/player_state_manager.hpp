#pragma once
#include "../Base/one_instance_singleton_base.hpp"

#include "../State/idle_state.hpp"
#include "../State/move_state.hpp"
#include "../State/squat_state.hpp"
#include "../State/run_state.hpp"
#include "../State/non_weapon_state.hpp"
#include "../State/aiming_state.hpp"
#include "../State/shot_state.hpp"
#include "../State/reload_state.hpp"

class PlayerStateManager final : public OneInstanceSingletonBase<PlayerStateManager>
{
public:
	PlayerStateManager();
	~PlayerStateManager();

	void Update();


private:
	std::shared_ptr<IState<Player>> m_current_basis_move_state;			// Šî‘b“I‚ÈˆÚ“®ó‘Ô
	std::shared_ptr<IState<Player>> m_current_extension_move_state;		// ’Ç‰Á‚ÌˆÚ“®ó‘Ô
	std::shared_ptr<IState<Player>> m_current_attack_state;				// UŒ‚‚ÉŠÖ‚·‚éó‘Ô

	std::shared_ptr<IdleState>		m_idle_state;
	std::shared_ptr<MoveState>		m_move_state;
	std::shared_ptr<SquatState>		m_squat_state;
	std::shared_ptr<RunState>		m_run_state;
	std::shared_ptr<NonWeaponState> m_non_weapon_state;
	std::shared_ptr<AimingState>	m_aiming_state;
	std::shared_ptr<ShotState>		m_shot_state;
	std::shared_ptr<ReloadState>	m_reload_state;
};
