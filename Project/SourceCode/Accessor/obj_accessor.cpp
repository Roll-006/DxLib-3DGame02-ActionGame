#include "../Base/obj_base.hpp"
#include "obj_accessor.hpp"

ObjAccessor::ObjAccessor()
{
	// 処理なし
}

ObjAccessor::~ObjAccessor()
{
	// 処理なし
}

void ObjAccessor::RemoveObj(const int obj_handle)
{
	m_objects.erase(obj_handle);
}
