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
