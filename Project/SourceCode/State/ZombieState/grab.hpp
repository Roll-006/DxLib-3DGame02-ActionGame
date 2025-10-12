#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/zombie.hpp"
#include "../../Part/zombie_state_controller.hpp"

#include "../../VirtualCamera/grab_virtual_camera_controller.hpp"

namespace zombie_state
{
	class Grab final : public ActionStateBase<Zombie>
	{
	public:
		Grab();
		~Grab() override;

		void Update		(std::shared_ptr<Zombie>& obj) override;
		void LateUpdate	(std::shared_ptr<Zombie>& obj) override;
		void Enter		(std::shared_ptr<Zombie>& obj) override;
		void Exit		(std::shared_ptr<Zombie>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Zombie>> ChangeState(std::shared_ptr<Zombie>& obj) override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		void DamageOverTime(std::shared_ptr<Zombie>& obj);

	private:
		static constexpr float kDamageOverTimeStartTime = 1.0f;		// 継続ダメージが開始するまでの時間
		static constexpr float kDamageIntervalTime		= 0.8f;
		static constexpr float kMaxGrabTime				= 6.6f;

		std::shared_ptr<GrabVirtualCameraController>	m_grab_camera_controller;

		float m_damage_over_time_start_timer;
		float m_damage_interval_timer;
		float m_grab_timer;
		bool  m_is_stop_all_state;
	};
}
