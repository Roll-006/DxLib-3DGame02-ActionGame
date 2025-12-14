#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class Reload final : public WeaponActionStateBase
	{
	public:
		Reload(Player& player);
		~Reload() override;

		void Update() override;
		void LateUpdate() override;
		void Enter() override;
		void Exit() override;

		[[nodiscard]] int GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		bool m_is_stop_all_state;

		bool m_is_release_ammo_box;
		bool m_is_set_ammo_box;
		bool m_is_cocking;
		Player& m_player;
	};
}
