#pragma once
#include "virtual_camera_base.hpp"

class ControlVirtualCameraBase abstract : public VirtualCameraBase
{
public:
	ControlVirtualCameraBase(const std::string& name, const VirtualCameraKind camera_kind) : 
		VirtualCameraBase	(name, camera_kind),
		m_angle				(v3d::GetZeroV())
	{ }

	virtual ~ControlVirtualCameraBase() = default;

	virtual void MoveUp()		abstract;
	virtual void MoveDown()		abstract;
	virtual void MoveLeft()		abstract;
	virtual void MoveRight()	abstract;
	virtual void InitAim()		abstract;

protected:
	VECTOR m_angle;
};
