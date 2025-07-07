#include "virtual_camera_base.hpp"

VirtualCameraBase::VirtualCameraBase(const std::string& name) :
	ObjBase	(name, ObjTag.VIRTUAL_CAMERA),
	m_body	(nullptr),
	m_aim	(nullptr),
	m_noise	(nullptr)
{

}
