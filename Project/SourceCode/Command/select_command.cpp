#include "select_command.hpp"

void select_cmd::Decide::Execute(ObjBase& obj)
{
	dynamic_cast<Selector*>(&obj);
}

void select_cmd::Back::Execute(ObjBase& obj)
{
	dynamic_cast<Selector*>(&obj);
}

void select_cmd::Up::Execute(ObjBase& obj)
{
	dynamic_cast<Selector*>(&obj);
}

void select_cmd::Down::Execute(ObjBase& obj)
{
	dynamic_cast<Selector*>(&obj);
}

void select_cmd::Left::Execute(ObjBase& obj)
{
	dynamic_cast<Selector*>(&obj);
}

void select_cmd::Right::Execute(ObjBase& obj)
{
	dynamic_cast<Selector*>(&obj);
}

void select_cmd::Pause::Execute(ObjBase& obj)
{
	dynamic_cast<Selector*>(&obj);
}
