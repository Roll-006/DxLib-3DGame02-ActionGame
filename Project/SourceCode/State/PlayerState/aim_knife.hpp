#pragma once
#include "../../Base/weapon_action_state_base.hpp"

#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"

namespace player_state
{
	class AimKnife final : public WeaponActionStateBase<Player>
	{
	public:
		AimKnife();
		~AimKnife() override;

		void Update		(Player* obj) override;
		void LateUpdate	(Player* obj) override;
		void Enter		(Player* obj) override;
		void Exit		(Player* obj) override;
		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(Player* obj) override;

	private:

	};
}
