#pragma once
#include "../../Base/zombie_state_base.hpp"

class Animator;

namespace zombie_state
{
	class GrabRun final : public ZombieStateBase
	{
	public:
		GrabRun(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator);
		~GrabRun();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const ZombieStateKind GetNextStateKind() override;

	private:
		static constexpr float kMaxTrackTime = 3.0f;

		float m_track_timer;
	};
}
