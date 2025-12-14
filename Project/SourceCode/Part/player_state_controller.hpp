#pragma once
#include <typeindex>

#include "../Interface/i_state_controller.hpp"
#include "../Kind/player_state_kind.hpp"

#include "../State/PlayerState/player_idle.hpp"
#include "../State/PlayerState/player_move.hpp"

#include "../State/PlayerState/player_action_null.hpp"
#include "../State/PlayerState/crouch.hpp"
#include "../State/PlayerState/player_run.hpp"
#include "../State/PlayerState/turn_around.hpp"
#include "../State/PlayerState/crouch_turn_around.hpp"
#include "../State/PlayerState/player_dead.hpp"
#include "../State/PlayerState/grabbed.hpp"
#include "../State/PlayerState/roundhouse_kick.hpp"
#include "../State/PlayerState/front_kick.hpp"
#include "../State/PlayerState/suplex.hpp"
#include "../State/PlayerState/escape.hpp"
#include "../State/PlayerState/stealth_kill.hpp"
#include "../State/PlayerState/finish_off.hpp"
#include "../State/PlayerState/escape_with_knife.hpp"
#include "../State/PlayerState/victory_pose.hpp"

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

class PlayerStateController final : public IStateController
{
public:
	PlayerStateController();
	~PlayerStateController() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;


	#pragma region Try判定
	[[nodiscard]] bool TryMove();
	[[nodiscard]] bool TryRun();
	[[nodiscard]] bool TryDead();
	[[nodiscard]] bool TryGrabbed();

	/// @brief WARNING : TryRoundhouseKickより前に呼び出す必要あり
	[[nodiscard]] bool TryFrontKick();
	/// @brief WARNING : TryFrontKickより後に呼び出す必要あり
	[[nodiscard]] bool TryRoundhouseKick();

	[[nodiscard]] bool TryStealthKill();
	[[nodiscard]] bool TryEquipKnifeShortcut();
	[[nodiscard]] bool TryAimKnife();
	[[nodiscard]] bool TryFirstSideSlashKnife();
	[[nodiscard]] bool TrySpinningSlash();
	[[nodiscard]] bool TryEquipGun();
	[[nodiscard]] bool TryEquipGunShortcut();
	[[nodiscard]] bool TryPullTrigger();
	[[nodiscard]] bool TryReload();
	[[nodiscard]] bool TryPullTriggerReload();
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] player_state::MoveStateKind			GetMoveStateKind		(const TimeKind time_kind)	const { return m_move_state.at(time_kind); }
	[[nodiscard]] player_state::ActionStateKind			GetActionStateKind		(const TimeKind time_kind)	const { return m_action_state.at(time_kind); }
	[[nodiscard]] player_state::WeaponActionStateKind	GetWeaponActionStateKind(const TimeKind time_kind)	const { return m_weapon_action_state.at(time_kind); }
	#pragma endregion

private:
	void CreateState()			override;
	void AddStopStatePair()		override;
	void AddCheckStopState()	override;

	/// @brief ステートを変更
	void ChangeState() override;

	/// @brief 変更するステートを生成
	[[nodiscard]] std::vector<int> CreateChangeState() override;

	/// @brief 未来のステート構成を生成
	[[nodiscard]] std::vector<int> CreateFutureState(const std::vector<int>& next_state) override;
	
	/// @brief ステートの停止処理
	void StopState(std::vector<int>& future_state, const std::shared_ptr<IState>& stop_state) override;

	void JudgeDestinationMoveState			(std::shared_ptr<IState>& stop_state);
	void JudgeDestinationActionState		(std::shared_ptr<IState>& stop_state);
	void JudgeDestinationWeaponActionState	(std::shared_ptr<IState>& stop_state);

private:
	std::unordered_map<std::type_index, std::shared_ptr<IState>>			m_states;						// 各ステート
	std::vector<int>														m_check_stop_state_handles;		// ステートの停止

	std::unordered_map<TimeKind, std::shared_ptr<MoveStateBase>>			m_move_state;					// 移動ステート
	std::unordered_map<TimeKind, std::shared_ptr<ActionStateBase>>			m_action_state;					// 行動ステート
	std::unordered_map<TimeKind, std::shared_ptr<WeaponActionStateBase>>	m_weapon_action_state;			// 武器に関するステート

	std::shared_ptr<WeaponActionStateBase>									m_change_weapon_after_state;	// 武器を取り換えた後に実行されるステート
};
