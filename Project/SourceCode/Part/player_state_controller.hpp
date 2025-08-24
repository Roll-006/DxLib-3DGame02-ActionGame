#pragma once
#include <typeindex>

#include "../Concept/state_concepts.hpp"
#include "../Data/Kind/player_state_kind.hpp"

#include "../State/PlayerState/move_null.hpp"
#include "../State/PlayerState/move.hpp"

#include "../State/PlayerState/action_null.hpp"
#include "../State/PlayerState/crouch.hpp"
#include "../State/PlayerState/run.hpp"
#include "../State/PlayerState/turn_around.hpp"
#include "../State/PlayerState/crouch_turn_around.hpp"

#include "../State/PlayerState/weapon_action_null.hpp"
#include "../State/PlayerState/attach_weapon.hpp"
#include "../State/PlayerState/detach_weapon.hpp"
#include "../State/PlayerState/equip_knife.hpp"
#include "../State/PlayerState/aim_knife.hpp"
#include "../State/PlayerState/stab_knife.hpp"
#include "../State/PlayerState/first_side_slash_knife.hpp"
#include "../State/PlayerState/second_side_slash_knife.hpp"
#include "../State/PlayerState/spinning_slash_knife.hpp"
#include "../State/PlayerState/parry.hpp"
#include "../State/PlayerState/equip_gun.hpp"
#include "../State/PlayerState/aim_gun.hpp"
#include "../State/PlayerState/shot.hpp"
#include "../State/PlayerState/shot_rocket_launcher.hpp"
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

	void Update		(std::shared_ptr<Player>);
	void LateUpdate	(std::shared_ptr<Player>);

	/// @brief ステートを取得
	template<typename StateT, typename ObjT>
	requires state_concepts::StateT<StateT, ObjT>
	[[nodiscard]] std::shared_ptr<StateT> GetState()
	{
		return m_states.count(typeid(StateT)) ? std::static_pointer_cast<StateT>(m_states.at(typeid(StateT))) : nullptr;
	}


	#pragma region Try判定
	[[nodiscard]] bool TryMove();
	[[nodiscard]] bool TryRun();
	[[nodiscard]] bool TryEquipKnifeShortcut(std::shared_ptr<Player>);
	[[nodiscard]] bool TrySpinningSlash();
	[[nodiscard]] bool TryEquipGun(std::shared_ptr<Player>);
	[[nodiscard]] bool TryEquipGunShortcut(std::shared_ptr<Player>);
	[[nodiscard]] bool TryPullTrigger(std::shared_ptr<Player>);
	[[nodiscard]] bool TryReload(std::shared_ptr<Player>);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] std::shared_ptr<MoveStateBase<Player>>		 GetMoveState		 (const TimeKind time_kind)	const { return m_move_state.at(time_kind); }
	[[nodiscard]] std::shared_ptr<ActionStateBase<Player>>		 GetActionState		 (const TimeKind time_kind)	const { return m_action_state.at(time_kind); }
	[[nodiscard]] std::shared_ptr<WeaponActionStateBase<Player>> GetWeaponActionState(const TimeKind time_kind)	const { return m_weapon_action_state.at(time_kind); }
	[[nodiscard]] std::shared_ptr<SpecialStateBase<Player>>		 GetSpecialState	 (const TimeKind time_kind)	const { return m_special_state.at(time_kind); }
	#pragma endregion

private:
	void CreateState();
	void AddStopStatePair();
	void AddCheckStopState();

	/// @brief ステートを変更
	void ChangeState(std::shared_ptr<Player> player);

	/// @brief 変更するステートを生成
	[[nodiscard]] std::vector<std::shared_ptr<IState<Player>>> CreateChangeState(std::shared_ptr<Player> player);

	/// @brief 未来のステート構成を生成
	[[nodiscard]] std::vector<std::shared_ptr<IState<Player>>> CreateFutureState(const std::vector<std::shared_ptr<IState<Player>>>& next_state);
	
	/// @brief ステートの停止処理
	void StopState(std::vector<std::shared_ptr<IState<Player>>>& future_state, const std::shared_ptr<IState<Player>> stop_state);

	void JudgeDestinationMoveState			(std::shared_ptr<IState<Player>>& stop_state);
	void JudgeDestinationActionState		(std::shared_ptr<IState<Player>>& stop_state);
	void JudgeDestinationWeaponActionState	(std::shared_ptr<IState<Player>>& stop_state);
	void JudgeDestinationSpecialState		(std::shared_ptr<IState<Player>>& stop_state);

private:
	std::unordered_map<std::type_index, std::shared_ptr<IState<Player>>>			m_states;						// 各ステート
	std::vector<int>																m_check_stop_state_handles;		// ステートの停止

	std::unordered_map<TimeKind, std::shared_ptr<MoveStateBase<Player>>>			m_move_state;					// 移動ステート
	std::unordered_map<TimeKind, std::shared_ptr<ActionStateBase<Player>>>			m_action_state;					// 行動ステート
	std::unordered_map<TimeKind, std::shared_ptr<WeaponActionStateBase<Player>>>	m_weapon_action_state;			// 武器に関するステート
	std::unordered_map<TimeKind, std::shared_ptr<SpecialStateBase<Player>>>			m_special_state;				// 特殊ステート

	std::shared_ptr<WeaponActionStateBase<Player>>									m_change_weapon_after_state;	// 武器を取り換えた後に実行されるステート
};
