#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class AimGun final : public WeaponActionStateBase
	{
	public:
		AimGun(Player& player);
		~AimGun() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		static constexpr float kWaitTime = 0.2f;

		bool	m_is_stop_all_state;
		float	m_elapsed_time;
		Player& m_player;
	};
}
