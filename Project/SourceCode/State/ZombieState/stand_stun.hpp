#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/zombie.hpp"
#include "../../Part/zombie_state_controller.hpp"

namespace zombie_state
{
	class StandStun final : public ActionStateBase<Zombie>
	{
	public:
		StandStun();
		~StandStun() override;

		void Update		(std::shared_ptr<Zombie>& obj) override;
		void LateUpdate	(std::shared_ptr<Zombie>& obj) override;
		void Enter		(std::shared_ptr<Zombie>& obj) override;
		void Exit		(std::shared_ptr<Zombie>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Zombie>> ChangeState(std::shared_ptr<Zombie>& obj) override;
		[[nodiscard]] bool IsAllStop() const override { return m_is_all_stop; }

	private:
		static constexpr float kStunTime = 5.0f;

		float m_stun_timer;
		bool  m_is_all_stop;
	};
}
