#include "camera_command.hpp"

void camera_cmd::MoveUp::Execute(ObjBase& obj)
{
	dynamic_cast<MainCamera*>(&obj)->MoveUp();
}

void camera_cmd::MoveDown::Execute(ObjBase& obj)
{
	dynamic_cast<MainCamera*>(&obj)->MoveDown();
}

void camera_cmd::MoveLeft::Execute(ObjBase& obj)
{
	dynamic_cast<MainCamera*>(&obj)->MoveLeft();
}

void camera_cmd::MoveRight::Execute(ObjBase& obj)
{
	dynamic_cast<MainCamera*>(&obj)->MoveRight();
}

void camera_cmd::InitAngle::Execute(ObjBase& obj)
{
	dynamic_cast<MainCamera*>(&obj)->InitAngle();
}
