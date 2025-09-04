#pragma once
#include "../Base/enemy_state_controller_base.hpp"
#include "../Data/Kind/zombie_state_kind.hpp"

#include "../State/ZombieState/wait.hpp"
#include "../State/ZombieState/patrol.hpp"
#include "../State/ZombieState/investigate.hpp"
#include "../State/ZombieState/search.hpp"
#include "../State/ZombieState/alert.hpp"
#include "../State/ZombieState/track.hpp"
#include "../State/ZombieState/battle.hpp"

#include "../State/ZombieState/zombie_move_null.hpp"
#include "../State/ZombieState/zombie_move.hpp"

#include "../State/ZombieState/zombie_action_null.hpp"
#include "../State/ZombieState/zombie_run.hpp"
#include "../State/ZombieState/grab.hpp"
#include "../State/ZombieState/grab_run.hpp"
#include "../State/ZombieState/knockback.hpp"
#include "../State/ZombieState/stand_stun.hpp"
#include "../State/ZombieState/crouch_stun.hpp"
#include "../State/ZombieState/play_dead.hpp"
#include "../State/ZombieState/dead.hpp"

class ZombieStateController final : public EnemyStateControllerBase<Zombie>
{
public:
	ZombieStateController();
	~ZombieStateController() override;

	void Update		(std::shared_ptr<Zombie> zombie) override;
	void LateUpdate	(std::shared_ptr<Zombie> zombie) override;

	/// @brief ステートを取得
	template<typename StateT, typename ObjT>
	requires state_concepts::StateT<StateT, ObjT>
	[[nodiscard]] std::shared_ptr<StateT> GetState()
	{
		return m_states.count(typeid(StateT)) ? std::static_pointer_cast<StateT>(m_states.at(typeid(StateT))) : nullptr;
	}


	#pragma region Try判定
	[[nodiscard]] bool TryTrack	(std::shared_ptr<Zombie> zombie);
	[[nodiscard]] bool TryRun	(std::shared_ptr<Zombie> zombie);
	[[nodiscard]] bool TryMove	();
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] std::shared_ptr<AIStateBase<Zombie>>		GetAIState		(const TimeKind time_kind)	const { return m_ai_state.at(time_kind); }
	[[nodiscard]] std::shared_ptr<MoveStateBase<Zombie>>	GetMoveState	(const TimeKind time_kind)	const { return m_move_state.at(time_kind); }
	[[nodiscard]] std::shared_ptr<ActionStateBase<Zombie>>	GetActionState	(const TimeKind time_kind)	const { return m_action_state.at(time_kind); }
	#pragma endregion

private:
	void CreateState()			override;
	void AddStopStatePair()		override;
	void AddCheckStopState()	override;

	/// @brief ステートを変更
	void ChangeState(std::shared_ptr<Zombie> zombie) override;

	/// @brief 変更するステートを生成
	[[nodiscard]] std::vector<std::shared_ptr<IState<Zombie>>> CreateChangeState(std::shared_ptr<Zombie> zombie) override;

	/// @brief 未来のステート構成を生成
	[[nodiscard]] std::vector<std::shared_ptr<IState<Zombie>>> CreateFutureState(const std::vector<std::shared_ptr<IState<Zombie>>>& next_state) override;
	
	/// @brief ステートの停止処理
	void StopState(std::vector<std::shared_ptr<IState<Zombie>>>& future_state, const std::shared_ptr<IState<Zombie>> stop_state) override;

	void JudgeDestinationAIState	(std::shared_ptr<IState<Zombie>>& stop_state);
	void JudgeDestinationMoveState	(std::shared_ptr<IState<Zombie>>& stop_state);
	void JudgeDestinationActionState(std::shared_ptr<IState<Zombie>>& stop_state);

private:
	std::unordered_map<std::type_index, std::shared_ptr<IState<Zombie>>>			m_states;						// 各ステート
	std::vector<int>																m_check_stop_state_handles;		// ステートの停止

	std::unordered_map<TimeKind, std::shared_ptr<AIStateBase<Zombie>>>				m_ai_state;						// AIステート
	std::unordered_map<TimeKind, std::shared_ptr<MoveStateBase<Zombie>>>			m_move_state;					// 移動ステート
	std::unordered_map<TimeKind, std::shared_ptr<ActionStateBase<Zombie>>>			m_action_state;					// 行動ステート
	//std::unordered_map<TimeKind, std::shared_ptr<WeaponActionStateBase<Player>>>	m_weapon_action_state;			// 武器に関するステート
};
