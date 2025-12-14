#pragma once
#include "../../Base/move_state_base.hpp"

#include "../../Object/zombie.hpp"
#include "../../Part/zombie_state_controller.hpp"
#include "../../Part/patrol_route_giver.hpp"

namespace zombie_state
{
	class Move final : public MoveStateBase
	{
	public:
		Move();
		~Move() override;

		void Update		(std::shared_ptr<Zombie>& obj) override;
		void LateUpdate	(std::shared_ptr<Zombie>& obj) override;
		void Enter		(std::shared_ptr<Zombie>& obj) override;
		void Exit		(std::shared_ptr<Zombie>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Zombie>> ChangeState(std::shared_ptr<Zombie>& obj) override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		bool m_is_stop_all_state;
	};
}
