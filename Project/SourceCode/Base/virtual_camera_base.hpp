#pragma once
#include "obj_base.hpp"

#include "../Part/camera_body.hpp"
#include "../Part/camera_aim.hpp"
#include "../Part/camera_noise.hpp"

class VirtualCameraBase abstract : public ObjBase
{
public:
	VirtualCameraBase(const std::string& name);
	virtual ~VirtualCameraBase() = default;

protected:
	std::shared_ptr<CameraBody>		m_body;
	std::shared_ptr<CameraAim>		m_aim;
	std::shared_ptr<CameraNoise>	m_noise;
};
