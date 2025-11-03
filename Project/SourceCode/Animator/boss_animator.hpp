#pragma once
#include "../Base/animator_base.hpp"
#include "../Kind/boss_anim_kind.hpp"

class BossStateController;

class BossAnimator final : public AnimatorBase
{
public:
	BossAnimator(const std::shared_ptr<Modeler>& modeler,  const std::shared_ptr<BossStateController>& state);
	~BossAnimator();

	void Init()		override;
	void Update()	override;

private:
	void LoadAnim()		override;
	void ChangeAnim()	override;


	#pragma region 状態の合成
	/// @brief MoveNull(Idle)ステートに、行動ステートを合成
	void CombineMoveNullWithAction();
	/// @brief Moveステートに、行動ステートを合成
	void CombineMoveWithAction();
	#pragma endregion

private:
	std::shared_ptr<BossStateController> m_state;
};
