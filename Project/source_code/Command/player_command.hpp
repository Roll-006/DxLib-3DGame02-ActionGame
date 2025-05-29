#pragma once
#include "../Base/command_base.hpp"

namespace player
{
	class Move final : public CommandBase
	{
	public:
		~Move()override;

		void Execute(const ObjBase& obj) override;
	};
}