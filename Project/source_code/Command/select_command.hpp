#pragma once
#include "../Base/command_base.hpp"

namespace select_command
{
	/// @brief 決定
	class Decide final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 戻る
	class Back final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 上選択
	class Up final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 下選択
	class Down final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 左選択
	class Left final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 右選択
	class Right final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief ポーズ
	class Pause final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};
}
