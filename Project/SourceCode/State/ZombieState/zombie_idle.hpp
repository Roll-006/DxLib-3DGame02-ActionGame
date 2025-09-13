#pragma once
#include "../../Base/move_state_base.hpp"

#include "../../Object/zombie.hpp"
#include "../../Part/zombie_state_controller.hpp"

namespace zombie_state
{
	class Idle final : public MoveStateBase<Zombie>
	{
	public:
		Idle();
		~Idle() override;

		void Update		(std::shared_ptr<Zombie> obj) override;
		void LateUpdate	(std::shared_ptr<Zombie> obj) override;
		void Enter		(std::shared_ptr<Zombie> obj) override;
		void Exit		(std::shared_ptr<Zombie> obj) override;

		[[nodiscard]] std::shared_ptr<IState<Zombie>> ChangeState(std::shared_ptr<Zombie> obj) override;

	private:

	};
}
