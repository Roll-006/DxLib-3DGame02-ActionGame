//#pragma once
//#include <typeindex>
//
//#include "../Concept/state_concepts.hpp"
//#include "../Data/Kind/zombie_state_kind.hpp"
//
//class ZombieStateController final
//{
//public:
//	ZombieStateController();
//	~ZombieStateController();
//
//	void Update		(Player* player);
//	void LateUpdate	(Player* player);
//
//	/// @brief ステートを取得
//	template<typename StateT, typename ObjT>
//	requires state_concepts::StateT<StateT, ObjT>
//	[[nodiscard]] std::shared_ptr<StateT> GetState()
//	{
//		return m_states.count(typeid(StateT)) ? std::static_pointer_cast<StateT>(m_states.at(typeid(StateT))) : nullptr;
//	}
//
//	#pragma region Try判定
//	[[nodiscard]] bool TryMove();
//	[[nodiscard]] bool TryRun();
//	[[nodiscard]] bool TryEquipKnifeShortcut(Player* player);
//	[[nodiscard]] bool TrySpinningSlash();
//	[[nodiscard]] bool TryEquipGun(Player* player);
//	[[nodiscard]] bool TryEquipGunShortcut(Player* player);
//	[[nodiscard]] bool TryPullTrigger(Player* player);
//	[[nodiscard]] bool TryReload(Player* player);
//	#pragma endregion
//
//
//	#pragma region Getter
//	[[nodiscard]] std::shared_ptr<MoveStateBase<Player>>		 GetMoveState		 (const TimeKind time_kind)	const { return m_move_state.at(time_kind); }
//	[[nodiscard]] std::shared_ptr<ActionStateBase<Player>>		 GetActionState		 (const TimeKind time_kind)	const { return m_action_state.at(time_kind); }
//	[[nodiscard]] std::shared_ptr<WeaponActionStateBase<Player>> GetWeaponActionState(const TimeKind time_kind)	const { return m_weapon_action_state.at(time_kind); }
//	[[nodiscard]] std::shared_ptr<SpecialStateBase<Player>>		 GetSpecialState	 (const TimeKind time_kind)	const { return m_special_state.at(time_kind); }
//	#pragma endregion
//
//private:
//	void CreateState();
//	void AddStopStatePair();
//	void AddCheckStopState();
//
//	/// @brief ステートを変更
//	void ChangeState(Player* player);
//
//	/// @brief 変更するステートを生成
//	[[nodiscard]] std::vector<std::shared_ptr<IState<Player>>> CreateChangeState(Player* player);
//
//	/// @brief 未来のステート構成を生成
//	[[nodiscard]] std::vector<std::shared_ptr<IState<Player>>> CreateFutureState(const std::vector<std::shared_ptr<IState<Player>>>& next_state);
//	
//	/// @brief ステートの停止処理
//	void StopState(std::vector<std::shared_ptr<IState<Player>>>& future_state, const std::shared_ptr<IState<Player>> stop_state);
//
//	void JudgeDestinationMoveState			(std::shared_ptr<IState<Player>>& stop_state);
//	void JudgeDestinationActionState		(std::shared_ptr<IState<Player>>& stop_state);
//	void JudgeDestinationWeaponActionState	(std::shared_ptr<IState<Player>>& stop_state);
//	void JudgeDestinationSpecialState		(std::shared_ptr<IState<Player>>& stop_state);
//
//private:
//	std::unordered_map<std::type_index, std::shared_ptr<IState<Player>>>			m_states;						// 各ステート
//	std::vector<int>																m_check_stop_state_handles;		// ステートの停止
//
//	std::unordered_map<TimeKind, std::shared_ptr<MoveStateBase<Player>>>			m_move_state;					// 移動ステート
//	std::unordered_map<TimeKind, std::shared_ptr<ActionStateBase<Player>>>			m_action_state;					// 行動ステート
//	std::unordered_map<TimeKind, std::shared_ptr<WeaponActionStateBase<Player>>>	m_weapon_action_state;			// 武器に関するステート
//	std::unordered_map<TimeKind, std::shared_ptr<SpecialStateBase<Player>>>			m_special_state;				// 特殊ステート
//
//	std::shared_ptr<WeaponActionStateBase<Player>>									m_change_weapon_after_state;	// 武器を取り換えた後に実行されるステート
//};
