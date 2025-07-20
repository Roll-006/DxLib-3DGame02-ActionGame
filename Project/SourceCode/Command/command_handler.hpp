#pragma once
#include "../Data/Kind/command_kind.hpp"
#include "../Base/singleton_base.hpp"
#include "../Input/input_checker.hpp"

class CommandHandler final : public SingletonBase<CommandHandler>
{
public:
	void Update();
	void LateUpdate();

	/// @brief コマンドを初期設定に戻す
	void InitKeyCommand();
	void InitPadCommand();

	/// @brief トリガーの入力回数をリセットする
	void InitTriggerInputCount(const CommandKind kind) { m_trigger_count.at(kind) = 0; }

	/// @brief トリガーの入力回数をカウントする
	//void CountUpTriggerInput(const CommandKind kind) { ++m_trigger_count.at(kind); }

	/// @brief 特殊コマンドの入力モードを設定する
	/// @param kind コマンドの種類
	/// @param input_mode 入力モード
	void SetInputModeKind(const CommandKind kind, const InputModeKind input_mode) { m_special_command.at(kind) = input_mode; }

	[[nodiscard]] InputModeKind GetInputModeKind(const CommandKind kind) const { return m_special_command.at(kind); }
	[[nodiscard]] int			GetTriggerCount (const CommandKind kind) const { return m_trigger_count.at(kind); }

	/// @brief 指定のコマンドが現在実行中かを判定
	[[nodiscard]] bool IsExecuting(const CommandKind kind);

private:
	CommandHandler();
	~CommandHandler() override;

	/// @brief コマンドに対応する入力コードの登録
	void AddInputCode(const CommandKind kind, const input_concepts::InputT auto& input_code);

	/// @brief コマンドに対応する入力コードの登録を解除
	void RemoveInputCode(const CommandKind kind, const input_concepts::InputT auto& input_code);

	/// @brief コマンドの実行判定
	void TryExecuteCommand(const std::vector<std::pair<CommandKind, InputCode>>& codes);

private:
	std::vector<CommandKind>							m_current_execute_command;		// 現在実行中のコマンド
	std::vector<std::pair<CommandKind, InputCode>>		m_key_codes;
	std::vector<std::pair<CommandKind, InputCode>>		m_pad_codes;

	std::unordered_map<CommandKind, InputModeKind>		m_special_command;				// 特殊コマンド
	std::unordered_map<CommandKind, int>				m_trigger_count;				// トリガー方式入力カウント

	friend SingletonBase<CommandHandler>;
};
