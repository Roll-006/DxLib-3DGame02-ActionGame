#pragma once
#include <typeindex>

#include "../Concept/state_concepts.hpp"
#include "../Data/Kind/zombie_state_kind.hpp"

#include "../State/ZombieState/patrol.hpp"

class ZombieStateController final
{
public:
	ZombieStateController();
	~ZombieStateController();

	void Update		(std::shared_ptr<Zombie> zombie);
	void LateUpdate	(std::shared_ptr<Zombie> zombie);

	/// @brief ステートを取得
	template<typename StateT, typename ObjT>
	requires state_concepts::StateT<StateT, ObjT>
	[[nodiscard]] std::shared_ptr<StateT> GetState()
	{
		return m_states.count(typeid(StateT)) ? std::static_pointer_cast<StateT>(m_states.at(typeid(StateT))) : nullptr;
	}


	#pragma region Try判定

	#pragma endregion


	#pragma region Getter
	[[nodiscard]] std::shared_ptr<ActionStateBase<Zombie>> GetActionState(const TimeKind time_kind)	const { return m_action_state.at(time_kind); }
	#pragma endregion

private:
	void CreateState();
	void AddStopStatePair();
	void AddCheckStopState();

	/// @brief ステートを変更
	void ChangeState(std::shared_ptr<Zombie> zombie);

	/// @brief 変更するステートを生成
	[[nodiscard]] std::vector<std::shared_ptr<IState<Zombie>>> CreateChangeState(std::shared_ptr<Zombie> zombie);

	/// @brief 未来のステート構成を生成
	[[nodiscard]] std::vector<std::shared_ptr<IState<Zombie>>> CreateFutureState(const std::vector<std::shared_ptr<IState<Zombie>>>& next_state);
	
	/// @brief ステートの停止処理
	void StopState(std::vector<std::shared_ptr<IState<Zombie>>>& future_state, const std::shared_ptr<IState<Zombie>> stop_state);

	void JudgeDestinationMoveState			(std::shared_ptr<IState<Zombie>>& stop_state);
	void JudgeDestinationActionState		(std::shared_ptr<IState<Zombie>>& stop_state);
	void JudgeDestinationWeaponActionState	(std::shared_ptr<IState<Zombie>>& stop_state);
	void JudgeDestinationSpecialState		(std::shared_ptr<IState<Zombie>>& stop_state);

private:
	std::unordered_map<std::type_index, std::shared_ptr<IState<Zombie>>>			m_states;						// 各ステート
	std::vector<int>																m_check_stop_state_handles;		// ステートの停止

	std::unordered_map<TimeKind, std::shared_ptr<ActionStateBase<Zombie>>>			m_action_state;					// 行動ステート
};
