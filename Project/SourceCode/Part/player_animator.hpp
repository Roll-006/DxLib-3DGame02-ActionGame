#pragma once
#include "../Base/animator_base.hpp"
#include "../Data/Kind/player_anim_kind.hpp"

class PlayerStateController;

class PlayerAnimator final : public AnimatorBase
{
public:
	PlayerAnimator(const std::shared_ptr<Modeler> modeler, const std::shared_ptr<PlayerStateController> state);
	~PlayerAnimator();

	void Init()		override;
	void Update()	override;

private:
	void LoadAnim() override;

	void ChangeAnim();


	#pragma region 状態の合成
	/// @brief MoveNull(Idle)ステートに、行動ステートを合成
	void CombineMoveNullWithAction();
	/// @brief Moveステートに、行動ステートを合成
	void CombineMoveWithAction();

	/// @brief MoveNull(Idle)とActionNullを合成した状態に、武器に関するステートを合成
	void CombineMoveNullActionNullWithWeaponAction();
	/// @brief MoveNull(Idle)とCrouchを合成した状態に、武器に関するステートを合成
	void CombineMoveNullCrouchWithWeaponAction();
	/// @brief MoveNull(Idle)とTurnAroundを合成した状態に、武器に関するステートを合成
	void CombineMoveNullTurnAroundWithWeaponAction();
	/// @brief MoveNull(Idle)とCrouchTurnAroundを合成した状態に、武器に関するステートを合成
	void CombineMoveNullCrouchTurnAroundWithWeaponAction();

	/// @brief MoveとActionNullを合成した状態に、武器に関するステートを合成
	void CombineMoveActionNullWithWeaponAction();
	/// @brief MoveとCrouchを合成した状態に、武器に関するステートを合成
	void CombineMoveCrouchWithWeaponAction();
	/// @brief MoveとRunを合成した状態に、武器に関するステートを合成
	void CombineMoveRunWithWeaponAction();
	#pragma endregion


	/// @brief 8方向移動を行うアニメーションのアタッチを行う
	/// @param forward_anim_kind 前方に移動するアニメーション
	void AttachAnimEightDir(const int forward_anim_kind);

private:
	std::shared_ptr<PlayerStateController> m_state;
};
