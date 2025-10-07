#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class EquipKnife final : public WeaponActionStateBase<Player>
	{
	public:
		EquipKnife();
		~EquipKnife() override;

		void Update		(std::shared_ptr<Player>& obj) override;
		void LateUpdate	(std::shared_ptr<Player>& obj) override;
		void Enter		(std::shared_ptr<Player>& obj) override;
		void Exit		(std::shared_ptr<Player>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player>& obj) override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		static constexpr float kReleaseKinfeForciblyTime = 5.0f;	// ナイフを手に持った状態を強制的に解除するまでの時間

		float m_elapsed_time;
		bool  m_is_stop_all_state;
	};
}
