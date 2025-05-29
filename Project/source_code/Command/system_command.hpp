#pragma once
#include "../Base/command_base.hpp"

namespace system
{
	class SystemCommand final : public CommandBase
	{
	public:
		~SystemCommand()override;

		void Execute(const ObjBase& obj) override;
	};
}