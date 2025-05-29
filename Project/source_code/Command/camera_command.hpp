#pragma once
#include "../Base/command_base.hpp"

namespace camera
{
	class CameraCommand final : public CommandBase
	{
	public:
		~CameraCommand()override;

		void Execute(const ObjBase& obj) override;
	};
}