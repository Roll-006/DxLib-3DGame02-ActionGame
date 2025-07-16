#pragma once
#include <typeindex>

#include "../Concept/state_concepts.hpp"

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

class PlayerStateController final
{
public:
	PlayerStateController();
	~PlayerStateController();

	void Update		(Player* player);
	void ChangeState(const Player* player);

	/// @brief ステートを取得
	template<typename StateT, typename ObjT>
	requires state_concepts::StateT<StateT, ObjT>
	[[nodiscard]] std::shared_ptr<StateT> GetState()
	{
		return m_states.count(typeid(StateT)) ? std::static_pointer_cast<StateT>(m_states.at(typeid(StateT))) : nullptr;
	}

	[[nodiscard]] std::shared_ptr<IMoveState<Player>>			GetCurrentMoveState()			const { return m_current_move_state; }
	[[nodiscard]] std::shared_ptr<IActionState<Player>>			GetCurrentActionState()			const { return m_current_action_state; }
	[[nodiscard]] std::shared_ptr<IWeaponActionState<Player>>	GetCurrentWeaponActionState()	const { return m_current_weapon_action_state; }
	[[nodiscard]] std::shared_ptr<ISpecialState<Player>>		GetCurrentSpecialState()		const { return m_current_special_state; }

private:
	std::unordered_map<std::type_index, std::shared_ptr<IState<Player>>> m_states;	// 各ステート(型情報をKeyとして使用)

	std::shared_ptr<IMoveState<Player>>				m_current_move_state;			// 移動ステート
	std::shared_ptr<IActionState<Player>>			m_current_action_state;			// 行動ステート
	std::shared_ptr<IWeaponActionState<Player>>		m_current_weapon_action_state;	// 武器に関するステート
	std::shared_ptr<ISpecialState<Player>>			m_current_special_state;		// 特殊ステート
};
