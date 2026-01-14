#include "obj_base.hpp"
#include "../Accessor/obj_accessor.hpp"

ObjBase::ObjBase(const std::string& name, const std::string& tag) :
	m_transform		(std::make_shared<Transform>()),
	m_name			(name),
	m_tag			(tag),
	//m_shadow_graphic(std::make_shared<Graphicer>(GraphicPath.SHADOW)),
	m_is_active		(true),
	m_obj_handle	(ObjAccessor::GetInstance()->CreateObjHandle())
{
	// 処理なし
}
