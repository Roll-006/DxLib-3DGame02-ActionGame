#include "camera_command.hpp"

void camera_cmd::MoveUp::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj)->MoveUp();
}

void camera_cmd::MoveDown::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj)->MoveDown();
}

void camera_cmd::MoveLeft::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj)->MoveLeft();
}

void camera_cmd::MoveRight::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj)->MoveRight();
}

void camera_cmd::InitAngle::Execute(ObjBase& obj)
{
	dynamic_cast<Camera*>(&obj)->InitAngle();
}
