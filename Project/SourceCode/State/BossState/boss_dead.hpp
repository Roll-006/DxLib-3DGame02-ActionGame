#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/boss.hpp"
#include "../../Part/boss_state_controller.hpp"

namespace boss_state
{
	class Dead final : public ActionStateBase<Boss>
	{
	public:
		Dead();
		~Dead() override;

		void Update		(std::shared_ptr<Boss>& obj) override;
		void LateUpdate	(std::shared_ptr<Boss>& obj) override;
		void Enter		(std::shared_ptr<Boss>& obj) override;
		void Exit		(std::shared_ptr<Boss>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Boss>> ChangeState(std::shared_ptr<Boss>& obj) override;
		[[nodiscard]] bool IsStopAllState() const override { return m_is_stop_all_state; }

	private:
		void ChangeMaterial(const auto model_handle, const float change_speed);

	private:
		static constexpr float kChangeColorWaitTime = 1.3f;
		static constexpr float kDisappearWaitTime	= 3.5f;

		bool	m_is_stop_all_state;
		float	m_elapsed_time;
		bool	m_is_seted_time_scale;
		float   m_change_color_wait_time;
		float	m_disappear_wait_time;
		bool	m_is_disappear_notified;

		MaterialData m_current_material;

		std::shared_ptr<DeadBossVirtualCamerasController> m_dead_cameras_controller;
	};
}
