#include "physics_manager.hpp"

PhysicsManager::PhysicsManager()
{
	// ˆ—‚È‚µ
}

PhysicsManager::~PhysicsManager()
{
	// ˆ—‚È‚µ
}

void PhysicsManager::Update()
{
	for (const auto& obj : m_physical_objects)
	{
		// d—Íˆ—‚ª‹–‰Â‚³‚ê‚Ä‚¢‚éê‡‚Ì‚Ýd—Í‚ð—^‚¦‚é
		if (IsApplyGravity(obj))
		{
			obj->ApplyGravity(kGravityAcceleration, kMaxGravity);
		}
	}
}

void PhysicsManager::RemovePhysicalObj				(const std::string& obj_name)
{
	const auto physical_obj = std::static_pointer_cast<PhysicalObjBase>(ObjManager::GetInstance()->GetObj(obj_name));

	if (std::find(m_physical_objects.begin(), m_physical_objects.end(), physical_obj) != m_physical_objects.end())
	{
		erase(m_physical_objects, physical_obj);
	}
}

void PhysicsManager::AddIgnoreObjPhysicalBehavior   (const std::string& obj_name)
{
	if (std::find(m_ignore_physical_behavior_obj_name.begin(), m_ignore_physical_behavior_obj_name.end(), obj_name) == m_ignore_physical_behavior_obj_name.end())
	{
		m_ignore_physical_behavior_obj_name.emplace_back(obj_name);
	}
}

void PhysicsManager::RemoveIgnoreObjPhysicalBehavior(const std::string& obj_name)
{
	if (std::find(m_ignore_physical_behavior_obj_name.begin(), m_ignore_physical_behavior_obj_name.end(), obj_name) != m_ignore_physical_behavior_obj_name.end())
	{
		erase(m_ignore_physical_behavior_obj_name, obj_name);
	}
}

void PhysicsManager::AddIgnoreObjGravity			(const std::string& obj_name)
{
	if (std::find(m_ignore_gravity_obj_name.begin(), m_ignore_gravity_obj_name.end(), obj_name) == m_ignore_gravity_obj_name.end())
	{
		m_ignore_gravity_obj_name.emplace_back(obj_name);
	}
}

void PhysicsManager::RemoveIgnoreObjGravity			(const std::string& obj_name)
{
	if (std::find(m_ignore_gravity_obj_name.begin(), m_ignore_gravity_obj_name.end(), obj_name) != m_ignore_gravity_obj_name.end())
	{
		erase(m_ignore_gravity_obj_name, obj_name);
	}
}
