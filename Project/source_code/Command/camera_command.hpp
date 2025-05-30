#pragma once
#include "../Base/command_base.hpp"
#include "../Object/camera.hpp"

namespace camera_cmd
{
	/// @brief 上移動
	class MoveUp final : public CommandBase
	{
	public:
		MoveUp() : CommandBase(CommandKind::kMoveUpCamera) {}
		void Execute(ObjBase& obj) override;
	};

	/// @brief 下移動
	class MoveDown final : public CommandBase
	{
	public:
		MoveDown() : CommandBase(CommandKind::kMoveDownCamera) {}
		void Execute(ObjBase& obj) override;
	};

	/// @brief 左移動
	class MoveLeft final : public CommandBase
	{
	public:
		MoveLeft() : CommandBase(CommandKind::kMoveLeftCamera) {}
		void Execute(ObjBase& obj) override;
	};

	/// @brief 右移動
	class MoveRight final : public CommandBase
	{
	public:
		MoveRight() : CommandBase(CommandKind::kMoveRightCamera) {}
		void Execute(ObjBase& obj) override;
	};

	/// @brief 視点リセット
	class InitAngle final : public CommandBase
	{
	public:
		InitAngle() : CommandBase(CommandKind::kInitAngle) {}
		void Execute(ObjBase& obj) override;
	};
}
