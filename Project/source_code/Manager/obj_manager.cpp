#include "../Base/obj_base.hpp"
#include "obj_manager.hpp"

ObjManager::ObjManager()
{
	// ˆ—‚È‚µ
}

ObjManager::~ObjManager()
{
	// ˆ—‚È‚µ
}

void ObjManager::RemoveObj(const std::string& obj_name)
{
	m_objects.erase(obj_name);
}

std::shared_ptr<ObjBase> ObjManager::GetObj(const std::string& obj_name)
{
	return m_objects.count(obj_name) ? m_objects.at(obj_name) : nullptr;
}
