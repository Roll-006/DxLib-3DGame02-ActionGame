#include "zombie_dead.hpp"

zombie_state::Dead::Dead() :
	ActionStateBase			(static_cast<int>(zombie_state::ActionStateKind::kDead)),
	m_is_stop_all_state		(false),
	m_elapsed_time_end_anim	(0.0f),
	m_change_color_wait_time(0.0f),
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
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		m_elapsed_time_end_anim += delta_time;
		if (m_elapsed_time_end_anim >= kReturnPoolTime)
		{
			CollisionManager::GetInstance()->RemoveCollideObj				(obj->GetObjHandle());
			PhysicsManager  ::GetInstance()->AddIgnoreObjPhysicalBehavior	(obj->GetObjHandle());
			PhysicsManager  ::GetInstance()->AddIgnoreObjGravity			(obj->GetObjHandle());
		
			obj->Disappear();
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
	m_current_material			= obj->GetModeler()->GetInitMaterialData();

	obj->RemoveCollider(ColliderKind::kCollider);
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
