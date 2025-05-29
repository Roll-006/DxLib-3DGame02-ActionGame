#pragma once
#include "../Base/command_base.hpp"
#include "../Object/camera.hpp"

namespace camera_command
{
	/// @brief 移動
	class Move final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 視点リセット
	class InitAngle final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};
}
