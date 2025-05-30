#include "select_command.hpp"

void select_cmd::Decide::Execute(ObjBase& obj)
{
	const auto selector = dynamic_cast<Selector*>(&obj);
}

void select_cmd::Back::Execute(ObjBase& obj)
{
	const auto selector = dynamic_cast<Selector*>(&obj);
}

void select_cmd::Up::Execute(ObjBase& obj)
{
	const auto selector = dynamic_cast<Selector*>(&obj);
}

void select_cmd::Down::Execute(ObjBase& obj)
{
	const auto selector = dynamic_cast<Selector*>(&obj);
}

void select_cmd::Left::Execute(ObjBase& obj)
{
	const auto selector = dynamic_cast<Selector*>(&obj);
}

void select_cmd::Right::Execute(ObjBase& obj)
{
	const auto selector = dynamic_cast<Selector*>(&obj);
}

void select_cmd::Pause::Execute(ObjBase& obj)
{
	const auto selector = dynamic_cast<Selector*>(&obj);
}
