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
	for (auto& obj : m_physical_objects)
	{
		obj->ApplyGravity(kGravityAcceleration, kMaxGravity);
	}
}

void PhysicsManager::RemovePhysicalObj(std::string obj_name)
{
	const auto physical_obj = std::static_pointer_cast<PhysicalObjBase>(ObjManager::GetInstance()->GetObj(obj_name));

	if (std::count(m_physical_objects.begin(), m_physical_objects.end(), physical_obj))
	{
		m_physical_objects.remove(physical_obj);
	}
}

void PhysicsManager::AddIgnoreObj(std::string obj_name)
{
	if (!std::count(m_ignore_obj_name.begin(), m_ignore_obj_name.end(), obj_name))
	{
		m_ignore_obj_name.emplace_back(obj_name);
	}
}

void PhysicsManager::RemoveIgnoreObj(std::string obj_name)
{
	if (std::count(m_ignore_obj_name.begin(), m_ignore_obj_name.end(), obj_name))
	{
		m_ignore_obj_name.remove(obj_name);
	}
}
