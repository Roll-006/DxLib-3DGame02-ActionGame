#include "../Base/obj_base.hpp"
#include "obj_manager.hpp"

ObjManager::ObjManager() : 
	m_handle_create_count(-1)
{
	// ˆ—‚È‚µ
}

ObjManager::~ObjManager()
{
	// ˆ—‚È‚µ
}

void ObjManager::RemoveObj(const int obj_handle)
{
	m_objects.erase(obj_handle);
}

std::shared_ptr<ObjBase> ObjManager::GetObj(const int obj_handle)
{
	return m_objects.count(obj_handle) ? m_objects.at(obj_handle) : nullptr;
}

std::shared_ptr<ObjBase> ObjManager::GetObj(const std::string& obj_name)
{
	for (const auto& obj : m_objects)
	{
		if (obj.second->GetName() == obj_name)
		{
			return obj.second;
		}
	}

	return nullptr;
}
