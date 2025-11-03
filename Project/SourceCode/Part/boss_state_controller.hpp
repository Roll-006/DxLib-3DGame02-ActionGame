#pragma once
#include "../Base/enemy_state_controller_base.hpp"
#include "../Kind/boss_state_kind.hpp"

#include "../State/BossState/boss_wait.hpp"
#include "../State/BossState/boss_patrol.hpp"
#include "../State/BossState/boss_investigate.hpp"
#include "../State/BossState/boss_search.hpp"
#include "../State/BossState/boss_alert.hpp"
#include "../State/BossState/boss_track.hpp"
#include "../State/BossState/boss_run_attack.hpp"
#include "../State/BossState/boss_close_range_attack.hpp"
#include "../State/BossState/boss_long_range_attack.hpp"

#include "../State/BossState/boss_idle.hpp"
#include "../State/BossState/boss_move.hpp"

#include "../State/BossState/boss_action_null.hpp"
#include "../State/BossState/boss_detected.hpp"
#include "../State/BossState/boss_run.hpp"
#include "../State/BossState/boss_stealth_killed.hpp"
#include "../State/BossState/boss_knockback.hpp"
#include "../State/BossState/boss_backward_knockback.hpp"
#include "../State/BossState/boss_stand_up.hpp"
#include "../State/BossState/boss_stand_stun.hpp"
#include "../State/BossState/boss_crouch_left_stun.hpp"
#include "../State/BossState/boss_crouch_right_stun.hpp"
#include "../State/BossState/boss_play_dead.hpp"
#include "../State/BossState/boss_dead.hpp"

class BossStateController final : public EnemyStateControllerBase<Boss>
{
public:
	BossStateController();
	~BossStateController() override;

	void Update		(std::shared_ptr<Boss> boss) override;
	void LateUpdate	(std::shared_ptr<Boss> boss) override;

	/// @brief ステートを取得
	template<typename StateT, typename ObjT>
	requires state_concepts::StateT<StateT, ObjT>
	[[nodiscard]] std::shared_ptr<StateT> GetState()
	{
		return m_states.count(typeid(StateT)) ? std::static_pointer_cast<StateT>(m_states.at(typeid(StateT))) : nullptr;
	}


	#pragma region Try判定
	[[nodiscard]] bool TryWaitForcibly		(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryPatrol			(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryTrack				(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryRunAttack			(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryMove				();
	[[nodiscard]] bool TryActionNullForcibly(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryDetected			(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryWalk				(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryRun				(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryStealthKilled		(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryGrabRun			();
	[[nodiscard]] bool TryKnockback			(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryDead				(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryLeftCrouchStun	(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryRightCrouchStun	(std::shared_ptr<Boss>& boss);
	[[nodiscard]] bool TryStandStun			(std::shared_ptr<Boss>& boss);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] std::shared_ptr<AIStateBase<Boss>>		GetAIState		(const TimeKind time_kind)	const { return m_ai_state.at(time_kind); }
	[[nodiscard]] std::shared_ptr<MoveStateBase<Boss>>		GetMoveState	(const TimeKind time_kind)	const { return m_move_state.at(time_kind); }
	[[nodiscard]] std::shared_ptr<ActionStateBase<Boss>>	GetActionState	(const TimeKind time_kind)	const { return m_action_state.at(time_kind); }
	#pragma endregion

private:
	void CreateState()			override;
	void AddStopStatePair()		override;
	void AddCheckStopState()	override;

	/// @brief ステートを変更
	void ChangeState(std::shared_ptr<Boss>& boss) override;

	/// @brief 変更するステートを生成
	[[nodiscard]] std::vector<std::shared_ptr<IState<Boss>>> CreateChangeState(std::shared_ptr<Boss>& boss) override;

	/// @brief 未来のステート構成を生成
	[[nodiscard]] std::vector<std::shared_ptr<IState<Boss>>> CreateFutureState(const std::vector<std::shared_ptr<IState<Boss>>>& next_state) override;
	
	/// @brief ステートの停止処理
	void StopState(std::vector<std::shared_ptr<IState<Boss>>>& future_state, const std::shared_ptr<IState<Boss>>& stop_state) override;

	void JudgeDestinationAIState	(std::shared_ptr<IState<Boss>>& stop_state);
	void JudgeDestinationMoveState	(std::shared_ptr<IState<Boss>>& stop_state);
	void JudgeDestinationActionState(std::shared_ptr<IState<Boss>>& stop_state);

private:
	std::unordered_map<std::type_index, std::shared_ptr<IState<Boss>>>		m_states;						// 各ステート
	std::vector<int>														m_check_stop_state_handles;		// ステートの停止

	std::unordered_map<TimeKind, std::shared_ptr<AIStateBase<Boss>>>		m_ai_state;						// AIステート
	std::unordered_map<TimeKind, std::shared_ptr<MoveStateBase<Boss>>>		m_move_state;					// 移動ステート
	std::unordered_map<TimeKind, std::shared_ptr<ActionStateBase<Boss>>>	m_action_state;					// 行動ステート
};
