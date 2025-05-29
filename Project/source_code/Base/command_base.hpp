#pragma once
#include "obj_base.hpp"

class CommandBase abstract
{
	virtual ~CommandBase() = default;

	virtual void Execute(const ObjBase& obj) abstract;
};
