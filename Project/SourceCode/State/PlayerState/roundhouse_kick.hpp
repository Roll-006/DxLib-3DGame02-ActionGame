#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class RoundhouseKick final : public ActionStateBase<Player>
	{
	public:
		RoundhouseKick();
		~RoundhouseKick() override;

		void Update		(std::shared_ptr<Player>& obj) override;
		void LateUpdate	(std::shared_ptr<Player>& obj) override;
		void Enter		(std::shared_ptr<Player>& obj) override;
		void Exit		(std::shared_ptr<Player>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player>& obj) override;
		[[nodiscard]] bool IsAllStop() const override { return m_is_all_stop; }

	private:
		static constexpr float kAttackTriggerRadius = 10.0f;

		bool m_is_all_stop;
		bool m_has_trigger_created;
		bool m_has_trigger_deleted;
	};
}
