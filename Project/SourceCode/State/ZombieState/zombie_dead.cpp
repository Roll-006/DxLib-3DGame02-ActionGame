#include "zombie_dead.hpp"

zombie_state::Dead::Dead() :
	ActionStateBase			(static_cast<int>(zombie_state::ActionStateKind::kDead)),
	m_is_stop_all_state		(false),
	m_elapsed_time_end_anim	(0.0f),
	m_change_color_wait_time(0.0f),
	m_is_start_disappear	(false),
	m_current_material		()
{

}

zombie_state::Dead::~Dead()
{

}

void zombie_state::Dead::Update(std::shared_ptr<Zombie>& obj)
{
	const auto delta_time = obj->GetDeltaTime();

	obj->DisallowStealthKill();

	// 色を黒に変化
	m_change_color_wait_time += delta_time;
	if (m_change_color_wait_time >= kChangeColorWaitTime)
	{
		ChangeMaterial(obj->GetModeler()->GetModelHandle(), 1.0f * delta_time);
	}

	// オブジェクトを下に落として消す
	m_elapsed_time_end_anim += delta_time;
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody) && !m_is_start_disappear)
	{
		if (m_elapsed_time_end_anim >= kDisappearWaitTime)
		{
			CollisionManager::GetInstance()->RemoveCollideObj(obj->GetObjHandle());
			PhysicsManager  ::GetInstance()->AddIgnoreObjPhysicalBehavior(obj->GetObjHandle());
			PhysicsManager  ::GetInstance()->AddIgnoreObjGravity(obj->GetObjHandle());

			// 離したことを演出カメラに通知
			const auto model_hanlde = obj->GetModeler()->GetModelHandle();
			auto	   hips_m		= MV1GetFrameLocalWorldMatrix(model_hanlde, MV1SearchFrame(model_hanlde, FramePath.HIPS));
			const auto hips_pos		= MGetTranslateElem(hips_m);
			const StartDisappearEnemyEvent event{ hips_pos - VGet(0.0f, 10.0f, 0.0f) };
			EventSystem::GetInstance()->Publish(event);

			m_is_start_disappear = true;
		}
	}

	if (m_is_start_disappear)
	{
		obj->Disappear();

		if (m_elapsed_time_end_anim >= kReturnPoolWaitTime)
		{
			obj->AllowReturnPool();
		}
	}
}

void zombie_state::Dead::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Dead::Enter(std::shared_ptr<Zombie>& obj)
{
	m_elapsed_time_end_anim		= 0.0f;
	m_change_color_wait_time	= 0.0f;
	m_is_start_disappear		= false;
	m_current_material			= MaterialData();

	obj->RemoveCollider(ColliderKind::kCollider);

	// 死亡したことを通知
	const DeadEnemyEvent event{ obj->GetEnemyID(), obj->GetModeler()->GetModelHandle() };
	EventSystem::GetInstance()->Publish(event);
}

void zombie_state::Dead::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Dead::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	return nullptr;
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
