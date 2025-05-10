#include "tracker_component.hpp"

TrackerComponent::TrackerComponent(GameObj* owner_obj) :
	ComponentBase		(owner_obj),
	m_speed				(0.0f),
	m_distance_to_target(0.0f),
	m_sync_pos			(false)
{
	m_target_pos[TimeState::kCurrect] = m_target_pos[TimeState::kNext] = v3d::GetZeroVector();
}

TrackerComponent::~TrackerComponent()
{

}

void TrackerComponent::Init()
{
	m_target_pos.at(TimeState::kNext) = m_target_pos.at(TimeState::kCurrect) = m_target_transform.at(TargetKind::kMain)->GetPos();
}

void TrackerComponent::Update()
{
	CalcTargetPos();
}

void TrackerComponent::LateUpdate()
{

}

void TrackerComponent::Draw()const
{

}

void TrackerComponent::AttachMainTarget(const std::shared_ptr<GameObj> obj)
{
	m_target_transform[TargetKind::kMain] = obj->GetComponent<TransformComponent>();
}

void TrackerComponent::AttachMainTarget(std::string& obj_name)
{
	auto target_obj = GameObjManager::GetInstance()->GetObj(obj_name);
	m_target_transform[TargetKind::kMain] = target_obj->GetComponent<TransformComponent>();
}

void TrackerComponent::DetachMainTarget()
{
	m_target_transform.erase(TargetKind::kMain);
}

//void TrackerComponent::AttachSubTarget(const std::shared_ptr<GameObj> obj)
//{
//	m_target_transform[TargetKind::kSub] = obj->GetComponent<TransformComponent>();
//}
//
//void TrackerComponent::AttachSubTarget(std::string& obj_name)
//{
//	auto target_obj = GameObjManager::GetInstance()->GetObj(obj_name);
//	m_target_transform[TargetKind::kSub] = target_obj->GetComponent<TransformComponent>();
//}
//
//void TrackerComponent::DetachSubTarget()
//{
//	m_target_transform.erase(TargetKind::kSub);
//}

VECTOR TrackerComponent::GetCurrentTargetPos()const
{
	return m_target_pos.at(TimeState::kCurrect);
}

VECTOR TrackerComponent::GetNextTargetPos()const
{
	return m_target_pos.at(TimeState::kNext);
}

TransformComponent* TrackerComponent::GetMainTargetTransform()const
{
	return m_target_transform.at(TargetKind::kMain).get();
}

//TransformComponent* TrackerComponent::GetSubTargetTransform()const
//{
//	return m_target_transform.at(TargetKind::kSub).get();
//}

void TrackerComponent::CalcTargetPos()
{
	m_target_pos.at(TimeState::kNext) = m_target_transform.at(TargetKind::kMain)->GetPos();

	VECTOR dir = m_target_pos.at(TimeState::kNext) - m_target_pos.at(TimeState::kCurrect);
	VECTOR vel = dir * m_speed * FPS::GetDeltaTime();

	m_target_pos.at(TimeState::kCurrect) += vel;

	if (VSize(m_target_pos.at(TimeState::kNext) - m_target_pos.at(TimeState::kCurrect)) < 0.1f)
	{
		m_target_pos.at(TimeState::kCurrect) = m_target_pos.at(TimeState::kNext);
	}
}
