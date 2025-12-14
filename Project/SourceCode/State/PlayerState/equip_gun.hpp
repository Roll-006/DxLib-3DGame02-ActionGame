#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class EquipGun final : public WeaponActionStateBase
	{
	public:
		EquipGun(Player& player);
		~EquipGun() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		static constexpr float kPossibleAimTime = 0.05f;		// エイミング状態への移行を許可するまでの時間

		float m_possible_aim_timer;
		bool  m_is_stop_all_state;
		Player& m_player;
	};
}
