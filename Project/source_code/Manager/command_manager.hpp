#pragma once
#include "../Command/select_command.hpp"
#include "../Command/camera_command.hpp"
#include "../Command/player_command.hpp"

class CommandManager final : public SingletonBase<CommandManager>
{
public:
	/// @brief コマンドを実行
	void Execute(const CommandKind command_kind, obj_concepts::ObjT auto& obj);

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
	std::vector<std::pair<CommandKind, InputCode>>	m_key_codes;
	std::vector<std::pair<CommandKind, InputCode>>	m_pad_codes;

	InputModeKind m_input_mode;		// ダッシュやしゃがみなどの入力方式

	friend SingletonBase<CommandManager>;
};
