#pragma once
#include "../Command/select_command.hpp"
#include "../Command/camera_command.hpp"
#include "../Command/player_command.hpp"

class CommandHandler final : public SingletonBase<CommandHandler>
{
public:
	enum class MoveKind
	{
		kRun,
		kSquat,
	};

public:
	void LateUpdate();

	/// @brief コマンドを実行
	void Execute(const CommandKind command_kind, obj_concepts::ObjT auto* obj)
	{
		// 対応するコマンドがない場合は早期return
		if (!m_commands.count(command_kind)) { return; }

		const auto input   = InputChecker::GetInstance();
		const auto command = m_commands.at(command_kind);
		std::vector<std::pair<CommandKind, InputCode>>* codes = &m_key_codes;

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
				// TODO : 条件式をmap化する
				if (input->IsInput(code.second))
				{
					m_current_frame_execute[code.first] = code.second;
					command->Execute(obj);

					// 2つめの同じコマンドは実行しない
					return;
				}
			}
		}
	}

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

	/// @brief コマンド読み込み
	void LoadSelectCommand();
	void LoadPlayerCommand();
	void LoadCameraCommand();

	/// @brief コマンドに対応する入力コードの登録
	void AddInputCode(const CommandKind kind, const input_concepts::InputT auto& input_code);

	/// @brief コマンドに対応する入力コードの登録を解除
	void RemoveInputCode(const CommandKind kind, const input_concepts::InputT auto& input_code);

private:
	std::unordered_map<CommandKind, std::shared_ptr<CommandBase>>	m_commands;
	std::unordered_map<CommandKind, InputCode>						m_current_frame_execute;		// 現在のフレームで実行されたコマンドに対応する入力コード
	std::vector<std::pair<CommandKind, InputCode>>					m_key_codes;
	std::vector<std::pair<CommandKind, InputCode>>					m_pad_codes;

	std::unordered_map<MoveKind, InputModeKind>						m_input_mode;					// 入力方式
	std::unordered_map<MoveKind, int>								m_trigger_count;				// トリガー方式入力カウント

	friend SingletonBase<CommandHandler>;
};
