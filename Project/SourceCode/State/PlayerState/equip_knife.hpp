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

		void Update		(Player* obj) override;
		void LateUpdate	(Player* obj) override;
		void Enter		(Player* obj) override;
		void Exit		(Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(Player* obj) override;

	private:
		static constexpr float kReleaseKinfeForciblyTime = 5.0f;	// ナイフを手に持った状態を強制的に解除するまでの時間

		float m_elapsed_time;
	};
}
