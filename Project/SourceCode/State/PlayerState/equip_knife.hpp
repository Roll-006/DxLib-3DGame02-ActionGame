#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class EquipKnife final : public WeaponActionStateBase
	{
	public:
		EquipKnife(Player& player);
		~EquipKnife() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		static constexpr float kReleaseKinfeForciblyTime = 5.0f;	// ナイフを手に持った状態を強制的に解除するまでの時間

		float m_elapsed_time;
		bool  m_is_stop_all_state;
		Player& m_player;
	};
}
