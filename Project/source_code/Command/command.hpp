#pragma once
#include "../Base/game_obj.hpp"

class Command
{
public:
	virtual ~Command() = default;

	virtual void Execute(const GameObj* obj) = 0;
};
