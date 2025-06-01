#include "camera_command.hpp"

void camera_cmd::MoveUp::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj);
}

void camera_cmd::MoveDown::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj);
}

void camera_cmd::MoveLeft::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj);
}

void camera_cmd::MoveRight::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj);
}

void camera_cmd::Approach::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj)->Approach();
}

void camera_cmd::Depart::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj)->Depart();
}

void camera_cmd::InitAngle::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj);
}
