#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class RoundhouseKick final : public ActionStateBase
	{
	public:
		RoundhouseKick(Player& player);
		~RoundhouseKick() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		static constexpr float kAttackTriggerRadius = 19.0f;

		bool m_is_stop_all_state;
		bool m_has_trigger_created;
		bool m_has_trigger_deleted;
		Player& m_player;
	};
}
