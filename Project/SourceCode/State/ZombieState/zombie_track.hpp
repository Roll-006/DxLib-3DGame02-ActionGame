#pragma once
#include "../../Base/zombie_state_base.hpp"

class Animator;

namespace zombie_state
{
	class Track final : public ZombieStateBase
	{
	public:
		Track(Zombie& zombie,
			zombie_state::State& state,
			const std::shared_ptr<Animator>& animator);
		~Track();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const ZombieStateKind GetNextStateKind() override;

	private:
		float m_in_sight_timer;
	};
}
