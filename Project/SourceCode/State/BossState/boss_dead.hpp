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
		static constexpr float kStartDisappearTime	= 2.3f;
		static constexpr float kChangeColorWaitTime = 0.8f;

		bool	m_is_stop_all_state;
		float	m_elapsed_time_end_anim;	// アニメーションの再生が終了してからの時間
		float   m_change_color_wait_time;
		bool	m_is_start_disappear;

		MaterialData m_current_material;
	};
}
