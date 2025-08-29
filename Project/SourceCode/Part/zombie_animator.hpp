#pragma once
#include "../Base/animator_base.hpp"
#include "../Data/Kind/zombie_anim_kind.hpp"

class ZombieStateController;

class ZombieAnimator final : public AnimatorBase
{
public:
	ZombieAnimator(const std::shared_ptr<Modeler> modeler,  const std::shared_ptr<ZombieStateController> state);
	~ZombieAnimator();

	void Init()		override;
	void Update()	override;

private:
	void LoadAnim() override;

	void ChangeAnim();

	void CombineMoveWithAction();

private:
	std::shared_ptr<ZombieStateController> m_state;
};
