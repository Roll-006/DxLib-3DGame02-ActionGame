#pragma once
#include "../Base/command_base.hpp"
#include "../Object/player.hpp"

namespace player_command
{
	/// @brief 移動
	class Move final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief ダッシュ
	class Run final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief しゃがむ
	class Squat final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 弾を撃つ
	class Shot final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief リロード
	class Reload final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};

	/// @brief 不意打ち
	class SilentKill final : public CommandBase
	{
	public:
		void Execute(const ObjBase& obj) override;
	};
}
