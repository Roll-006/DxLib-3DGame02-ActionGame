#pragma once
#include "../Base/animator_base.hpp"
#include "../Data/Kind/player_kind.hpp"

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

private:
	std::shared_ptr<PlayerStateController> m_state;
};
