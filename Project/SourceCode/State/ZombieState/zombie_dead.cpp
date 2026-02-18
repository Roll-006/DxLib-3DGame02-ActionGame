#include "../../Object/zombie.hpp"
#include "../../Kind/zombie_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/zombie_state_kind.hpp"
#include "../../Data/material_data.hpp"
#include "zombie_state.hpp"
#include "zombie_dead.hpp"

zombie_state::Dead::Dead(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator) :
	ZombieStateBase			(zombie, state, animator, ZombieStateKind::kIdle),
	m_elapsed_time_end_anim	(0.0f),
	m_change_color_wait_time(0.0f),
	m_is_start_disappear	(false),
	m_current_material		()
{

}

zombie_state::Dead::~Dead()
{

}

void zombie_state::Dead::Update()
{
	m_zombie.CalcMoveSpeedStop();
	m_zombie.InitMoveOffset();
	m_zombie.DisallowStealthKill();
	m_zombie.DisallowDisperseEnemy();

	const auto delta_time = m_zombie.GetDeltaTime();

	// 色を黒に変化
	m_change_color_wait_time += delta_time;
	if (m_change_color_wait_time >= kChangeColorWaitTime)
	{
		ChangeMaterial(m_zombie.GetModeler()->GetModelHandle(), 1.0f * delta_time);
	}

	// オブジェクトを下に落として消す
	m_elapsed_time_end_anim += delta_time;
	if (m_zombie.GetAnimator()->IsPlayEnd(Animator::BodyKind::kUpperBody) && !m_is_start_disappear)
	{
		if (m_elapsed_time_end_anim >= kDisappearWaitTime)
		{
			CollisionManager::GetInstance()->RemoveCollideObj(m_zombie.GetObjHandle());
			PhysicsManager  ::GetInstance()->AddIgnoreObjPhysicalBehavior(m_zombie.GetObjHandle());
			PhysicsManager  ::GetInstance()->AddIgnoreObjGravity(m_zombie.GetObjHandle());

			// 離したことを演出カメラに通知
			const auto model_hanlde = m_zombie.GetModeler()->GetModelHandle();
			auto	   hips_m		= MV1GetFrameLocalWorldMatrix(model_hanlde, MV1SearchFrame(model_hanlde, FramePath.HIPS));
			const auto hips_pos		= matrix::GetPos(hips_m);
			const StartDisappearEnemyEvent event{ hips_pos - VGet(0.0f, 10.0f, 0.0f) };
			EventSystem::GetInstance()->Publish(event);

			m_is_start_disappear = true;
		}
	}

	if (m_is_start_disappear)
	{
		m_zombie.Disappear();

		if (m_elapsed_time_end_anim >= kReturnPoolWaitTime)
		{
			m_zombie.AllowReturnPool();
		}
	}

	m_zombie.UpdateLocomotion();

	const auto prev_state_kind = m_state.GetPrevStateKind();
	if (prev_state_kind != ZombieStateKind::kKnockback && prev_state_kind != ZombieStateKind::kStealthKilled)
	{
		m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kDead));
	}
}

void zombie_state::Dead::LateUpdate()
{

}

void zombie_state::Dead::Enter()
{
	m_elapsed_time_end_anim		= 0.0f;
	m_change_color_wait_time	= 0.0f;
	m_is_start_disappear		= false;
	m_current_material			= MaterialData();

	m_zombie.RemoveCollider(ColliderKind::kCollider);

	// 死亡したことを通知
	EventSystem::GetInstance()->Publish(DeadEnemyEvent(m_zombie.GetEnemyID(), m_zombie.GetModeler()->GetModelHandle()));
}

void zombie_state::Dead::Exit()
{

}

const ZombieStateKind zombie_state::Dead::GetNextStateKind()
{
	return ZombieStateKind::kNone;
}

void zombie_state::Dead::ChangeMaterial(const auto model_handle, const float change_speed)
{
	math::Decrease(m_current_material.diffuse_color.r,  change_speed, 0.0f);
	math::Decrease(m_current_material.diffuse_color.g,  change_speed, 0.0f);
	math::Decrease(m_current_material.diffuse_color.b,  change_speed, 0.0f);

	math::Decrease(m_current_material.specular_color.r, change_speed, 0.0f);
	math::Decrease(m_current_material.specular_color.g, change_speed, 0.0f);
	math::Decrease(m_current_material.specular_color.b, change_speed, 0.0f);

	math::Decrease(m_current_material.emissive_color.r, change_speed, 0.0f);
	math::Decrease(m_current_material.emissive_color.g, change_speed, 0.0f);
	math::Decrease(m_current_material.emissive_color.b, change_speed, 0.0f);

	math::Decrease(m_current_material.ambient_color.r,  change_speed, 0.0f);
	math::Decrease(m_current_material.ambient_color.g,  change_speed, 0.0f);
	math::Decrease(m_current_material.ambient_color.b,  change_speed, 0.0f);

	MV1SetDifColorScale(model_handle, m_current_material.diffuse_color );
	MV1SetSpcColorScale(model_handle, m_current_material.specular_color);
	MV1SetEmiColorScale(model_handle, m_current_material.emissive_color);
	MV1SetAmbColorScale(model_handle, m_current_material.ambient_color );
}
