#pragma once
#include "../Command/select_command.hpp"
#include "../Command/camera_command.hpp"
#include "../Command/player_command.hpp"

class CommandManager final : public SingletonBase<CommandManager>
{
public:
	/// @brief コマンドを実行
	void Execute(const CommandKind command_kind, obj_concepts::ObjT auto& obj)
	{
		// 対応するコマンドがない場合は早期return
		if (!m_commands.count(command_kind)) { return; }

		const auto input = InputChecker::GetInstance();
		const auto command = m_commands.at(command_kind);
		std::vector<std::pair<CommandKind, InputCode>>* codes = nullptr;

		// 現在の入力デバイスと同じ入力以外は実行しない
		switch (input->GetCurrentInputDevice())
		{
		case DeviceKind::kKeyboard: codes = &m_key_codes;	break;
		case DeviceKind::kPad:	    codes = &m_pad_codes;	break;
		}

		// コマンドに対応する入力コードが見つかった場合実行
		for (const auto& code : *codes)
		{
			if (code.first == command_kind)
			{
				// 2つめの同じコマンドは実行しない
				if (input->IsInput(code.second))
				{
					command->Execute(obj);
					return;
				}
			}
		}
	}

	/// @brief コマンドを初期設定に戻す
	void InitKeyCommand();
	void InitPadCommand();

	[[nodiscard]] InputModeKind GetInputModeKind()const { return m_input_mode; }

private:
	CommandManager();
	~CommandManager() override;

	/// @brief コマンド読み込み
	void LoadSelectCommand();
	void LoadPlayerCommand();
	void LoadCameraCommand();

	/// @brief コマンドに対応する入力コードの登録
	void AddInputCode(const CommandKind kind, const input_concepts::InputT auto& input_code);

	/// @brief コマンドに対応する入力コードの登録を解除
	void RemoveInputCode(const CommandKind kind, const input_concepts::InputT auto& input_code);

private:
	std::unordered_map<CommandKind, std::shared_ptr<CommandBase>> m_commands;
	std::vector<std::pair<CommandKind, InputCode>> m_key_codes;
	std::vector<std::pair<CommandKind, InputCode>> m_pad_codes;

	InputModeKind m_input_mode;		// ダッシュやしゃがみなどの入力方式

	friend SingletonBase<CommandManager>;
};
