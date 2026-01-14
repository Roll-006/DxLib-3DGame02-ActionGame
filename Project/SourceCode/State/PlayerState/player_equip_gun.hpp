#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
	class EquipGun final : public PlayerStateBase
	{
	public:
		EquipGun(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator);
		~EquipGun();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const PlayerStateKind GetNextStateKind() override;

	private:
		static constexpr float kPossibleAimTime = 0.05f;

		float m_possible_aim_timer;
	};
}
