#pragma once
#include "obj_base.hpp"

#include "../Part/camera_aim.hpp"
#include "../Part/camera_body.hpp"
#include "../Part/camera_noise.hpp"

class VirtualCameraBase abstract : public ObjBase
{
public:
	VirtualCameraBase();
	virtual ~VirtualCameraBase() = default;

protected:

};