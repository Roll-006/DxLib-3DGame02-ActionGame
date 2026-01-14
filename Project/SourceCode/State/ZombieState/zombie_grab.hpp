#pragma once
#include "../../Base/zombie_state_base.hpp"

class Animator;

namespace zombie_state
{
	class Grab final : public ZombieStateBase
	{
	public:
		Grab(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator);
		~Grab();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const ZombieStateKind GetNextStateKind() override;

	private:
		void DamageOverTime();

	private:
		static constexpr float kDamageOverTimeStartTime = 1.0f;	// 継続ダメージ開始時間
		static constexpr float kDamageIntervalTime = 0.8f;
		static constexpr float kMaxGrabTime = 6.6f;

		std::shared_ptr<GrabVirtualCameraController> m_grab_camera_controller;

		float m_damage_over_time_start_timer;
		float m_damage_interval_timer;
		float m_grab_timer;
	};
}
