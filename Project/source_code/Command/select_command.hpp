#pragma once
#include "../Base/command_base.hpp"
#include "../Object/selector.hpp"

namespace select_cmd
{
	/// @brief 決定
	class Decide final : public CommandBase
	{
	public:
		Decide() : CommandBase(CommandKind::kDecide){}
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 戻る
	class Back final : public CommandBase
	{
	public:
		Back() : CommandBase(CommandKind::kBack) {}
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 上選択
	class Up final : public CommandBase
	{
	public:
		Up() : CommandBase(CommandKind::kSelectUp) {}
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 下選択
	class Down final : public CommandBase
	{
	public:
		Down() : CommandBase(CommandKind::kSelectDown) {}
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 左選択
	class Left final : public CommandBase
	{
	public:
		Left() : CommandBase(CommandKind::kSelectLeft) {}
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 右選択
	class Right final : public CommandBase
	{
	public:
		Right() : CommandBase(CommandKind::kSelectRight) {}
		void Execute(const ObjBase& obj) override;
	};

	/// @brief ポーズ
	class Pause final : public CommandBase
	{
	public:
		Pause() : CommandBase(CommandKind::kPause) {}
		void Execute(const ObjBase& obj) override;
	};
}
