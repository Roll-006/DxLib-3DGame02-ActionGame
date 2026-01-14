#pragma once
#include "../../Base/zombie_state_base.hpp"

class Animator;
struct MaterialData;

namespace zombie_state
{
	class Dead final : public ZombieStateBase
	{
	public:
		Dead(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator);
		~Dead();

		void Update()		override;
		void LateUpdate()	override;
		void Enter()		override;
		void Exit()			override;

		[[nodiscard]] const ZombieStateKind GetNextStateKind() override;

	private:
		void ChangeMaterial(const auto model_handle, const float change_speed);

	private:
		static constexpr float kDisappearWaitTime	= 2.3f;
		static constexpr float kChangeColorWaitTime = 0.8f;
		static constexpr float kReturnPoolWaitTime	= 15.0f;

		float	m_elapsed_time_end_anim;	// アニメーションの再生が終了してからの時間
		float   m_change_color_wait_time;
		bool	m_is_start_disappear;

		MaterialData m_current_material;
	};
}
