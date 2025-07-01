#pragma once
#include "../Base/animator_base.hpp"
#include "../Data/Kind/player_kind.hpp"

class PlayerAnimator final : public AnimatorBase
{
public:
	PlayerAnimator(const std::shared_ptr<Modeler> modeler);
	~PlayerAnimator();

private:
	void LoadAnim() override;
};
