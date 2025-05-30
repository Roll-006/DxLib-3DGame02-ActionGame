#include "camera_command.hpp"

void camera_cmd::MoveUp::Execute(ObjBase& obj)
{
	const auto camera = dynamic_cast<Camera*>(&obj);
}

void camera_cmd::MoveDown::Execute(ObjBase& obj)
{
	const auto camera = dynamic_cast<Camera*>(&obj);
}

void camera_cmd::MoveLeft::Execute(ObjBase& obj)
{
	const auto camera = dynamic_cast<Camera*>(&obj);
}

void camera_cmd::MoveRight::Execute(ObjBase& obj)
{
	const auto camera = dynamic_cast<Camera*>(&obj);
}

void camera_cmd::InitAngle::Execute(ObjBase& obj)
{
	const auto camera = dynamic_cast<Camera*>(&obj);
}
