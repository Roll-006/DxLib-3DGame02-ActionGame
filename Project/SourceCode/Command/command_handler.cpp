#include "command_handler.hpp"

CommandHandler::CommandHandler()
{
	m_special_command[CommandKind::kRun] = m_special_command[CommandKind::kCrouch] = InputModeKind::kTrigger;
	m_trigger_count	 [CommandKind::kRun] = m_trigger_count  [CommandKind::kCrouch] = 0;

	// 初期設定
	InitKeyCommand();
	InitPadCommand();
}

CommandHandler::~CommandHandler()
{

}

void CommandHandler::Update()
{
	const auto input = InputChecker::GetInstance();

	// 現在の入力デバイスに合わせて処理を実行
	switch (input->GetCurrentInputDevice())
	{
	case DeviceKind::kKeyboard:
		TryExecuteCommand(m_key_codes);
		break;

	case DeviceKind::kPad:
		TryExecuteCommand(m_pad_codes);
		break;
	}
}

void CommandHandler::LateUpdate()
{
	m_current_execute_command.clear();
}

void CommandHandler::InitKeyCommand()
{
	m_key_codes.clear();

	// TODO : 後にJson化
	AddInputCode(CommandKind::kDecide,			KEY_INPUT_SPACE);
	AddInputCode(CommandKind::kDecide,			KEY_INPUT_RETURN);
	AddInputCode(CommandKind::kBack,			mouse::ButtonKind::kRight);
	AddInputCode(CommandKind::kSelectUp,		KEY_INPUT_W);
	AddInputCode(CommandKind::kSelectUp,		KEY_INPUT_UP);
	AddInputCode(CommandKind::kSelectUp,		mouse::WheelKind::kUp);
	AddInputCode(CommandKind::kSelectDown,		KEY_INPUT_S);
	AddInputCode(CommandKind::kSelectDown,		KEY_INPUT_DOWN);
	AddInputCode(CommandKind::kSelectDown,		mouse::WheelKind::kDown);
	AddInputCode(CommandKind::kSelectLeft,		KEY_INPUT_A);
	AddInputCode(CommandKind::kSelectLeft,		KEY_INPUT_LEFT);
	AddInputCode(CommandKind::kSelectRight,		KEY_INPUT_D);
	AddInputCode(CommandKind::kSelectRight,		KEY_INPUT_RIGHT);
	AddInputCode(CommandKind::kPause,			KEY_INPUT_ESCAPE);
	AddInputCode(CommandKind::kPause,			KEY_INPUT_TAB);

	AddInputCode(CommandKind::kAimGun,			mouse::ButtonKind::kRight);
	AddInputCode(CommandKind::kAttack,			mouse::ButtonKind::kLeft);
	AddInputCode(CommandKind::kAimKnife,		KEY_INPUT_SPACE);
	AddInputCode(CommandKind::kInvestigate,		KEY_INPUT_F);
	AddInputCode(CommandKind::kMelee,			KEY_INPUT_F);
	AddInputCode(CommandKind::kReload,			KEY_INPUT_R);
	AddInputCode(CommandKind::kScope,			KEY_INPUT_LALT);
	AddInputCode(CommandKind::kMoveUpPlayer,	KEY_INPUT_W);
	AddInputCode(CommandKind::kMoveDownPlayer,	KEY_INPUT_S);
	AddInputCode(CommandKind::kMoveLeftPlayer,	KEY_INPUT_A);
	AddInputCode(CommandKind::kMoveRightPlayer,	KEY_INPUT_D);
	AddInputCode(CommandKind::kRun,				KEY_INPUT_LSHIFT);
	AddInputCode(CommandKind::kCrouch,			KEY_INPUT_E);
	AddInputCode(CommandKind::kQuickTurn,		KEY_INPUT_Q);

	AddInputCode(CommandKind::kMoveUpCamera,	KEY_INPUT_UP);
	AddInputCode(CommandKind::kMoveDownCamera,	KEY_INPUT_DOWN);
	AddInputCode(CommandKind::kMoveLeftCamera,	KEY_INPUT_LEFT);
	AddInputCode(CommandKind::kMoveRightCamera,	KEY_INPUT_RIGHT);

	// 例外処理として実行
	//AddInputCode(CommandKind::kMoveUpCamera,		mouse::SlideDirKind::kUp);
	//AddInputCode(CommandKind::kMoveDownCamera,	mouse::SlideDirKind::kDown);
	//AddInputCode(CommandKind::kMoveLeftCamera,	mouse::SlideDirKind::kLeft);
	//AddInputCode(CommandKind::kMoveRightCamera,	mouse::SlideDirKind::kRight);
}

void CommandHandler::InitPadCommand()
{
	m_pad_codes.clear();

	// TODO : 後にJson化
	AddInputCode(CommandKind::kDecide,			pad::ButtonKind	::kA);
	AddInputCode(CommandKind::kBack,			pad::ButtonKind	::kB);
	AddInputCode(CommandKind::kSelectUp,		pad::ButtonKind	::kUp);
	AddInputCode(CommandKind::kSelectUp,		pad::StickKind	::kLSUp);
	AddInputCode(CommandKind::kSelectDown,		pad::ButtonKind	::kDown);
	AddInputCode(CommandKind::kSelectDown,		pad::StickKind	::kLSDown);
	AddInputCode(CommandKind::kSelectLeft,		pad::ButtonKind	::kLeft);
	AddInputCode(CommandKind::kSelectLeft,		pad::StickKind	::kLSLeft);
	AddInputCode(CommandKind::kSelectRight,		pad::ButtonKind	::kRight);
	AddInputCode(CommandKind::kSelectRight,		pad::StickKind	::kLSRight);
	AddInputCode(CommandKind::kPause,			pad::ButtonKind	::kStart);

	AddInputCode(CommandKind::kAimGun,			pad::TriggerKind::kLT);
	AddInputCode(CommandKind::kAttack,			pad::TriggerKind::kRT);
	AddInputCode(CommandKind::kAimKnife,		pad::ButtonKind	::kLB);
	AddInputCode(CommandKind::kInvestigate,		pad::ButtonKind	::kA);
	AddInputCode(CommandKind::kMelee,			pad::ButtonKind	::kA);
	AddInputCode(CommandKind::kReload,			pad::ButtonKind	::kX);
	AddInputCode(CommandKind::kScope,			pad::ButtonKind	::kLSPush);
	AddInputCode(CommandKind::kMoveUpPlayer,	pad::StickKind	::kLSUp);
	AddInputCode(CommandKind::kMoveDownPlayer,	pad::StickKind	::kLSDown);
	AddInputCode(CommandKind::kMoveLeftPlayer,	pad::StickKind	::kLSLeft);
	AddInputCode(CommandKind::kMoveRightPlayer, pad::StickKind	::kLSRight);
	AddInputCode(CommandKind::kRun,				pad::ButtonKind	::kLSPush);
	AddInputCode(CommandKind::kRun,				pad::ButtonKind	::kRB);
	AddInputCode(CommandKind::kCrouch,			pad::ButtonKind	::kB);
	AddInputCode(CommandKind::kQuickTurn,		pad::ButtonKind	::kRB);

	AddInputCode(CommandKind::kInitAim,			pad::ButtonKind	::kRB);

	// 例外処理として実行
	//AddInputCode(CommandKind::kMoveUpCamera,		pad::StickKind	::kRSUp);
	//AddInputCode(CommandKind::kMoveDownCamera,	pad::StickKind	::kRSDown);
	//AddInputCode(CommandKind::kMoveLeftCamera,	pad::StickKind	::kRSLeft);
	//AddInputCode(CommandKind::kMoveRightCamera,	pad::StickKind	::kRSRight);
}

bool CommandHandler::IsExecutingCommand(const CommandKind kind)
{
	// 特殊コマンドのトリガー方式の場合、入力カウントによって実行されたかを判定
	if (m_special_command.count(kind))
	{
		const auto input = InputChecker::GetInstance();
		int count = 0;
		switch (GetInputModeKind(kind))
		{
		case InputModeKind::kTrigger:
			return GetTriggerCount(kind) % 2 == 1 ? true : false;
			break;

		default:
			break;
		}
	}

	// 通常コマンド・特殊コマンドのホールド方式
	return std::find(m_current_execute_command.begin(), m_current_execute_command.end(), kind) != m_current_execute_command.end();
}

void CommandHandler::AddInputCode(const CommandKind kind, const input_concepts::InputT auto& input_code)
{
	const auto input	= InputChecker::GetInstance();
	const auto code		= input->ConvertInputTemplateToInputCode(input_code);
	std::vector<std::pair<CommandKind, InputCode>>* codes = nullptr;
	
	switch (input->GetInputKind(input_code))
	{
	case InputKind::kKey:
	case InputKind::kMouseButton:
	case InputKind::kMouseWheel:
	case InputKind::kMouseSlide:
		codes = &m_key_codes;
		break;

	case InputKind::kPadButton:
	case InputKind::kPadTrigger:
	case InputKind::kPadStick:
		codes = &m_pad_codes;
		break;
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

void CommandHandler::RemoveInputCode(const CommandKind kind, const input_concepts::InputT auto& input_code)
{
	const auto input = InputChecker::GetInstance();
	const auto code  = input->ConvertInputTemplateToInputCode(input_code);
	std::vector<std::pair<CommandKind, InputCode>>* codes = nullptr;

	switch (input->GetInputKind(input_code))
	{
	case InputKind::kKey:
	case InputKind::kMouseButton:
	case InputKind::kMouseWheel:
	case InputKind::kMouseSlide:
		codes = &m_key_codes;
		break;

	case InputKind::kPadButton:
	case InputKind::kPadTrigger:
	case InputKind::kPadStick:
		codes = &m_pad_codes;
		break;
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

void CommandHandler::TryExecuteCommand(const std::vector<std::pair<CommandKind, InputCode>>& codes)
{
	const auto input = InputChecker::GetInstance();

	// 入力された情報を保存
	std::vector<CommandKind> executed_command;

	for (const auto& code : codes)
	{
		if (input->IsInput(code.second))
		{
			// 入力されていないコマンドのみ格納
			if (std::find(executed_command.begin(), executed_command.end(), code.first) == executed_command.end())
			{
				// 特殊コマンドのトリガー方式であった場合、入力回数をカウント
				if (m_special_command.count(code.first) && input->GetInputState(code.second) == InputState::kSingle)
				{
					CountUpTrigger(code.first);
				}

				executed_command		 .emplace_back(code.first);
				m_current_execute_command.emplace_back(code.first);
			}
		}
	}
}
