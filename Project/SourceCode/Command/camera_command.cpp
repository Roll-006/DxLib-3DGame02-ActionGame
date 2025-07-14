#include "camera_command.hpp"

void control_camera_cmd::MoveUp::Execute(ObjBase* obj)
{
	static_cast<ControlVirtualCameraBase*>(obj)->MoveUp();
}

void control_camera_cmd::MoveDown::Execute(ObjBase* obj)
{
	static_cast<ControlVirtualCameraBase*>(obj)->MoveDown();
}

void control_camera_cmd::MoveLeft::Execute(ObjBase* obj)
{
	static_cast<ControlVirtualCameraBase*>(obj)->MoveLeft();
}

void control_camera_cmd::MoveRight::Execute(ObjBase* obj)
{
	static_cast<ControlVirtualCameraBase*>(obj)->MoveRight();
}

void control_camera_cmd::InitAim::Execute(ObjBase* obj)
{
	static_cast<ControlVirtualCameraBase*>(obj)->InitAim();
}
