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

private:
	CommandManager();
	~CommandManager() override;

	/// @brief コマンド読み込み
	void LoadSelectCommand();
	void LoadPlayerCommand();
	void LoadCameraCommand();

	/// @brief コマンドに対応する入力の登録
	template<input_concepts::InputT InputT>
	void AddInputCode(const CommandKind kind, const InputT& input_code)
	{
		// 対応するコマンドがない場合は早期return
		if (!m_commands.count(kind)) { return; }

		const auto input = InputChecker::GetInstance();
		const auto code	 = input->ConvertInputTemplateToInputCode(input_code);
		std::vector<std::pair<CommandKind, InputCode>>* codes = nullptr;
		
		switch (input->GetCurrentInputDevice())
		{
		case DeviceKind::kKeyboard: codes = &m_key_codes; break;
		case DeviceKind::kPad:		codes = &m_pad_codes; break;
		}

		// 新規データのみ追加する
		const auto add = std::find_if(codes->begin(), codes->end(), [=](const std::pair<CommandKind, InputCode> p)
		{
			return p.first == kind && p.second.kind == code.kind && p.second.code == code.code;
		});

		if (add == codes->end())
		{
			codes->emplace_back(std::make_pair(kind, code));
		}
	}

	/// @brief コマンドに対応する入力の登録を解除
	template<input_concepts::InputT InputT>
	void RemoveInputCode(const CommandKind kind, const InputT& input_code)
	{
		// 対応するコマンドがない場合は早期return
		if (!m_commands.count(kind)) { return; }

		const auto input = InputChecker::GetInstance();
		const auto code  = input->ConvertInputTemplateToInputCode(input_code);
		std::vector<std::pair<CommandKind, InputCode>>* codes = nullptr;

		switch (input->GetCurrentInputDevice())
		{
		case DeviceKind::kKeyboard: codes = &m_key_codes; break;
		case DeviceKind::kPad:		codes = &m_pad_codes; break;
		}

		// 削除する入力コードを検索
		const auto remove = std::find_if(codes->begin(), codes->end(), [=](const std::pair<CommandKind, InputCode> p)
		{
			return p.first == kind && p.second.kind == code.kind && p.second.code == code.code;
		});

		// 一致する入力コードを削除
		if (remove != codes->end())
		{
			codes->erase(remove);
		}
	}

private:
	std::unordered_map<CommandKind, std::shared_ptr<CommandBase>> m_commands;
	std::vector<std::pair<CommandKind, InputCode>>	m_key_codes;
	std::vector<std::pair<CommandKind, InputCode>>	m_pad_codes;

	friend SingletonBase<CommandManager>;
};
