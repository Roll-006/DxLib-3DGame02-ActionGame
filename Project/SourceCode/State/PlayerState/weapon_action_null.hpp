#pragma once
#include "../../Base/weapon_action_state_base.hpp"

namespace player_state
{
	class WeaponActionNull final : public WeaponActionStateBase
	{
	public:
		WeaponActionNull(Player& player);
		~WeaponActionNull() override;

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] int  GetNextStateKind() override;
		[[nodiscard]] bool IsStopAllState()	const override { return m_is_stop_all_state; }

	private:
		bool m_is_stop_all_state;
		Player& m_player;
	};
}
