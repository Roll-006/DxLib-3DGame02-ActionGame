#pragma once
#include "../../Base/zombie_state_base.hpp"

class Animator;

namespace zombie_state
{
	class StandStun final : public ZombieStateBase
	{
	public:
		StandStun(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator);
		~StandStun();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const ZombieStateKind GetNextStateKind() override;

	private:
		static constexpr float kStunTime = 5.0f;

		float m_stun_timer;
	};
}
