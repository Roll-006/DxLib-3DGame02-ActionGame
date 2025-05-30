#pragma once
#include "../Command/select_command.hpp"
#include "../Command/camera_command.hpp"
#include "../Command/player_command.hpp"

class CommandManager final : public SingletonBase<CommandManager>
{
public:
	void Execute(const CommandKind command_kind, obj_concepts::ObjT auto& obj)
	{
		const DeviceKind device = InputChecker::GetInstance()->GetCurrentInputDevice();

		// 指定のコマンドを検索し実行
		switch (device)
		{
		case DeviceKind::kKeyboard:
			for (const auto& [cmd_k, input_c, cmd] : m_key_command)
			{
				if (command_kind == cmd_k) { cmd->Execute(obj); }
			}
			break;

		case DeviceKind::kPad:
			for (const auto& [cmd_k, input_c, cmd] : m_pad_command)
			{
				if (command_kind == cmd_k) { cmd->Execute(obj); }
			}
			break;
		}
	}

	/// @brief コマンドを初期設定に戻す
	void InitKeyCommand();
	void InitPadCommand();

private:
	CommandManager();
	~CommandManager() override;

	void LoadSelectCommand();
	void LoadPlayerCommand();
	void LoadCameraCommand();

	template<input_concepts::InputT InputT>
	void AddCommand(const CommandKind kind, const InputT& input_code, std::shared_ptr<CommandBase> command)
	{
		InputCode code;

		// キー
		if (std::is_same_v<int, InputT>)
		{
			code = InputCode(InputKind::kKey, static_cast<int>(input_code));
			m_key_command.emplace_back(std::make_tuple(kind, code, command));
		}
		// マウスボタン
		if (std::is_same_v<mouse::ButtonKind, InputT>)
		{
			code = InputCode(InputKind::kMouseButton, static_cast<int>(input_code));
			m_key_command.emplace_back(std::make_tuple(kind, code, command));
		}
		// マウスホイール
		if (std::is_same_v<mouse::WheelKind, InputT>)
		{
			code = InputCode(InputKind::kMouseWheel, static_cast<int>(input_code));
			m_key_command.emplace_back(std::make_tuple(kind, code, command));
		}
		// マウススライド
		if (std::is_same_v<mouse::SlideDirKind, InputT>)
		{
			code = InputCode(InputKind::kMouseSlide, static_cast<int>(input_code));
			m_key_command.emplace_back(std::make_tuple(kind, code, command));
		}
		// パッドボタン
		if (std::is_same_v<pad::ButtonKind, InputT>)
		{
			code = InputCode(InputKind::kPadButton, static_cast<int>(input_code));
			m_pad_command.emplace_back(std::make_tuple(kind, code, command));
		}
		// パッドトリガー
		if (std::is_same_v<pad::TriggerKind, InputT>)
		{
			code = InputCode(InputKind::kPadTrigger, static_cast<int>(input_code));
			m_pad_command.emplace_back(std::make_tuple(kind, code, command));
		}
		// パッドスティック
		if (std::is_same_v<pad::StickKind, InputT>)
		{
			code = InputCode(InputKind::kPadStick, static_cast<int>(input_code));
			m_pad_command.emplace_back(std::make_tuple(kind, code, command));
		}
	}

private:
	std::vector<std::shared_ptr<CommandBase>> m_commands;
	std::vector<std::tuple<CommandKind, InputCode, std::shared_ptr<CommandBase>>> m_key_command;
	std::vector<std::tuple<CommandKind, InputCode, std::shared_ptr<CommandBase>>> m_pad_command;

	friend SingletonBase<CommandManager>;
};
