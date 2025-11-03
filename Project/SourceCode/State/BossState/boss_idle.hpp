#pragma once
#include "../../Base/move_state_base.hpp"

#include "../../Object/boss.hpp"
#include "../../Part/boss_state_controller.hpp"

namespace boss_state
{
	class Idle final : public MoveStateBase<Boss>
	{
	public:
		Idle();
		~Idle() override;

		void Update		(std::shared_ptr<Boss>& obj) override;
		void LateUpdate	(std::shared_ptr<Boss>& obj) override;
		void Enter		(std::shared_ptr<Boss>& obj) override;
		void Exit		(std::shared_ptr<Boss>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Boss>> ChangeState(std::shared_ptr<Boss>& obj) override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		bool m_is_stop_all_state;
	};
}
