#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/boss.hpp"
#include "../../Part/boss_state_controller.hpp"

namespace boss_state
{
	class CrouchRightStun final : public ActionStateBase<Boss>
	{
	public:
		CrouchRightStun();
		~CrouchRightStun() override;

		void Update		(std::shared_ptr<Boss>& obj) override;
		void LateUpdate	(std::shared_ptr<Boss>& obj) override;
		void Enter		(std::shared_ptr<Boss>& obj) override;
		void Exit		(std::shared_ptr<Boss>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Boss>> ChangeState(std::shared_ptr<Boss>& obj) override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		static constexpr float kStunTime = 5.0f;

		float m_stun_timer;
		bool  m_is_stop_all_state;
	};
}
