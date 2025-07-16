#pragma once
#include "../../Interface/i_weapon_action_state.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class GunEquipped final : public IWeaponActionState<Player>
	{
	public:
		GunEquipped();
		~GunEquipped() override;

		void Update(Player* obj) override;
		void Enter (const Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(const Player* obj) override;

	private:

	};
}
