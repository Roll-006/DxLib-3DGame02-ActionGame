#pragma once
#include "obj_base.hpp"

class CommandBase abstract
{
public:
	virtual void Execute(const ObjBase& obj) abstract;
};
