#pragma once
#include "../Base/singleton_base.hpp"

class Debugger final : public SingletonBase<Debugger>
{
public:

private:
	Debugger();
	~Debugger() override;

	friend SingletonBase<Debugger>;
};
