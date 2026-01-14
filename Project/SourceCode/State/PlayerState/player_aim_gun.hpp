#pragma once
#include "../../Base/player_state_base.hpp"

class Animator;

namespace player_state
{
	class AimGun final : public PlayerStateBase
	{
	public:
		AimGun(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator);
		~AimGun();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const PlayerStateKind GetNextStateKind() override;

	private:
		static constexpr float kWaitTime = 0.2f;

		float m_elapsed_time;
	};
}