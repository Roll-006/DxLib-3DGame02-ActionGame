#pragma once
#include "../../Base/action_state_base.hpp"

#include "../../Object/zombie.hpp"
#include "../../Part/zombie_state_controller.hpp"

namespace zombie_state
{
	class Dead final : public ActionStateBase<Zombie>
	{
	public:
		Dead();
		~Dead() override;

		void Update		(std::shared_ptr<Zombie>& obj) override;
		void LateUpdate	(std::shared_ptr<Zombie>& obj) override;
		void Enter		(std::shared_ptr<Zombie>& obj) override;
		void Exit		(std::shared_ptr<Zombie>& obj) override;

		[[nodiscard]] std::shared_ptr<IState<Zombie>> ChangeState(std::shared_ptr<Zombie>& obj) override;
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
