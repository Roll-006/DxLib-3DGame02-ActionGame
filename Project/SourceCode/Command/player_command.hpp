#pragma once
#include "../Base/command_base.hpp"
#include "../Object/player.hpp"

namespace player_cmd
{
	/// @brief 移動
	class MoveUp final : public CommandBase
	{
	public:
		MoveUp() : CommandBase(CommandKind::kMoveUpPlayer) {}
		void Execute(ObjBase* obj) override;
	};

	class MoveDown final : public CommandBase
	{
	public:
		MoveDown() : CommandBase(CommandKind::kMoveDownPlayer) {}
		void Execute(ObjBase* obj) override;
	};

	class MoveLeft final : public CommandBase
	{
	public:
		MoveLeft() : CommandBase(CommandKind::kMoveLeftPlayer) {}
		void Execute(ObjBase* obj) override;
	};

	class MoveRight final : public CommandBase
	{
	public:
		MoveRight() : CommandBase(CommandKind::kMoveRightPlayer) {}
		void Execute(ObjBase* obj) override;
	};

	/// @brief ダッシュ
	class Run final : public CommandBase
	{
	public:
		Run() : CommandBase(CommandKind::kRun) {}
		void Execute(ObjBase* obj) override;
	};

	/// @brief しゃがむ
	class Squat final : public CommandBase
	{
	public:
		Squat() : CommandBase(CommandKind::kSquat) {}
		void Execute(ObjBase* obj) override;
	};

	/// @brief 弾を撃つ
	class Shot final : public CommandBase
	{
	public:
		Shot() : CommandBase(CommandKind::kShot) {}
		void Execute(ObjBase* obj) override;
	};

	/// @brief 銃を構える
	class Aiming final : public CommandBase
	{
	public:
		Aiming() : CommandBase(CommandKind::kAimingGun) {}
		void Execute(ObjBase* obj) override;
	};

	/// @brief リロード
	class Reload final : public CommandBase
	{
	public:
		Reload() : CommandBase(CommandKind::kReloadGun) {}
		void Execute(ObjBase* obj) override;
	};

	/// @brief 不意打ち
	class SilentKill final : public CommandBase
	{
	public:
		SilentKill() : CommandBase(CommandKind::kSilentKill) {}
		void Execute(ObjBase* obj) override;
	};

	/// @brief 振り向き
	class TurnAround final : public CommandBase
	{
	public:
		TurnAround() : CommandBase(CommandKind::kTurnAround) {}
		void Execute(ObjBase* obj) override;
	};
}
