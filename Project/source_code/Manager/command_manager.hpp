#pragma once
#include <memory>
#include <unordered_map>

#include "../Data/Kind/command_kind.hpp"
//#include "../Command/select_command.hpp"
//#include "../Command/camera_command.hpp"
//#include "../Command/player_command.hpp"

#include "../Data/input_data.hpp"
#include "../Base/singleton_base.hpp"

class CommandManager final : public SingletonBase<CommandManager>
{
public:
	void Update(const CommandKind command_kind);

private:
	CommandManager();
	~CommandManager();

private:
	/// @brief ボタンに対応するコマンドのペア
	struct CommandPair
	{
		std::pair<InputCode, std::shared_ptr<CommandBase>> pair;
	};

	std::unordered_map<CommandKind, CommandPair> m_command;

	friend SingletonBase<CommandManager>;
};