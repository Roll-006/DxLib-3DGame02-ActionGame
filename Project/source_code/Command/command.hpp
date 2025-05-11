#pragma once
#include "../Base/obj_base.hpp"

class Command
{
public:
	virtual ~Command() = default;

	virtual void Execute(const ObjBase* obj) = 0;
};
