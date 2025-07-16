#pragma once
#include "../Data/Kind/command_kind.hpp"
#include "../Base/singleton_base.hpp"
#include "../Input/input_checker.hpp"

class CommandHandler final : public SingletonBase<CommandHandler>
{
public:
	enum class MoveKind
	{
		kRun,
		kCrouch,
	};

public:
	void Update();
	void LateUpdate();

	/// @brief コマンドを初期設定に戻す
	void InitKeyCommand();
	void InitPadCommand();

	/// @brief トリガーの入力回数をリセットする
	void InitTriggerCount(const MoveKind kind) { m_trigger_count.at(kind) = 0; }

	/// @brief トリガーの入力回数をカウントする
	void CountUpTrigger(const MoveKind kind) { ++m_trigger_count.at(kind); }

	[[nodiscard]] InputModeKind GetInputModeKind(const MoveKind kind) const { return m_input_mode.at(kind); }
	[[nodiscard]] int			GetTriggerCount (const MoveKind kind) const { return m_trigger_count.at(kind); }

	/// @brief 現在のフレームで実行されたコマンドに対応する入力コードを取得
	/// @return 存在した場合 : 対応コード, 存在しない場合 : nullptr
	[[nodiscard]] InputCode* GetCurrentFrameExecuteInputCode(const CommandKind kind)
	{
		return m_current_frame_execute.count(kind) ? &m_current_frame_execute.at(kind) : nullptr;
	}

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
	std::unordered_map<CommandKind, InputCode>			m_current_frame_execute;		// 現在のフレームで実行されたコマンドに対応する入力コード
	std::vector<std::pair<CommandKind, InputCode>>		m_key_codes;
	std::vector<std::pair<CommandKind, InputCode>>		m_pad_codes;

	std::unordered_map<MoveKind, InputModeKind>			m_input_mode;					// 入力方式
	std::unordered_map<MoveKind, int>					m_trigger_count;				// トリガー方式入力カウント

	friend SingletonBase<CommandHandler>;
};
