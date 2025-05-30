#include "command_manager.hpp"

CommandManager::CommandManager()
{
	LoadSelectCommand();
	LoadPlayerCommand();
	LoadCameraCommand();

	// 初期設定
	// TODO : 後にJsonで設定を保持できるよう変更
	InitKeyCommand();
	InitPadCommand();
}

CommandManager::~CommandManager()
{

}

void CommandManager::Execute(const CommandKind command_kind, obj_concepts::ObjT auto& obj)
{
	// 対応するコマンドがない場合は早期return
	if (!m_commands.count(command_kind)) { return; }

	const auto input   = InputChecker::GetInstance();
	const auto command = m_commands.at(command_kind);
	std::vector<std::pair<CommandKind, InputCode>>* codes = nullptr;

	// 現在の入力デバイスと同じ入力以外は実行しない
	switch (input->GetCurrentInputDevice())
	{
	case DeviceKind::kKeyboard: codes = &m_key_codes; break;
	case DeviceKind::kPad:		codes = &m_pad_codes; break;
	}

	// コマンドに対応する入力コードをすべて検索し実行
	for (const auto& code : codes)
	{
		if (code.first == command_kind)
		{
			if (input->IsInput(code.second)) { command->Execute(obj); }
		}
	}
}

void CommandManager::LoadSelectCommand()
{
	m_commands[CommandKind::kDecide]			= (std::make_shared<select_cmd::Decide>());
	m_commands[CommandKind::kBack]				= (std::make_shared<select_cmd::Back>());
	m_commands[CommandKind::kSelectUp]			= (std::make_shared<select_cmd::Up>());
	m_commands[CommandKind::kSelectDown]		= (std::make_shared<select_cmd::Down>());
	m_commands[CommandKind::kSelectLeft]		= (std::make_shared<select_cmd::Left>());
	m_commands[CommandKind::kSelectRight]		= (std::make_shared<select_cmd::Right>());
	m_commands[CommandKind::kPause]				= (std::make_shared<select_cmd::Pause>());
}

void CommandManager::LoadPlayerCommand()
{
	m_commands[CommandKind::kMoveUpPlayer]		= (std::make_shared<player_cmd::MoveUp>());
	m_commands[CommandKind::kMoveDownPlayer]	= (std::make_shared<player_cmd::MoveDown>());
	m_commands[CommandKind::kMoveLeftPlayer]	= (std::make_shared<player_cmd::MoveLeft>());
	m_commands[CommandKind::kMoveRightPlayer]	= (std::make_shared<player_cmd::MoveRight>());
	m_commands[CommandKind::kRun]				= (std::make_shared<player_cmd::Run>());
	m_commands[CommandKind::kSquat]				= (std::make_shared<player_cmd::Squat>());
	m_commands[CommandKind::kShot]				= (std::make_shared<player_cmd::Shot>());
	m_commands[CommandKind::kReadyGun]			= (std::make_shared<player_cmd::Ready>());
	m_commands[CommandKind::kReloadGun]			= (std::make_shared<player_cmd::Reload>());
	m_commands[CommandKind::kSilentKill]		= (std::make_shared<player_cmd::SilentKill>());
	m_commands[CommandKind::kQuickTurn]			= (std::make_shared<player_cmd::QuickTurn>());
}

void CommandManager::LoadCameraCommand()
{
	// TODO : 後にJson化
	m_commands[CommandKind::kMoveUpCamera]		= (std::make_shared<camera_cmd::MoveUp>());
	m_commands[CommandKind::kMoveDownCamera]	= (std::make_shared<camera_cmd::MoveDown>());
	m_commands[CommandKind::kMoveLeftCamera]	= (std::make_shared<camera_cmd::MoveLeft>());
	m_commands[CommandKind::kMoveRightCamera]	= (std::make_shared<camera_cmd::MoveRight>());
	m_commands[CommandKind::kInitAngle]			= (std::make_shared<camera_cmd::InitAngle>());
}

void CommandManager::InitKeyCommand()
{
	m_key_codes.clear();

	// TODO : 後にJson化
	AddInputCode(CommandKind::kDecide,			KEY_INPUT_SPACE);
	AddInputCode(CommandKind::kDecide,			KEY_INPUT_F);
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
	AddInputCode(CommandKind::kMoveUpPlayer,	KEY_INPUT_W);
	AddInputCode(CommandKind::kMoveDownPlayer,	KEY_INPUT_S);
	AddInputCode(CommandKind::kMoveLeftPlayer,	KEY_INPUT_A);
	AddInputCode(CommandKind::kMoveRightPlayer,	KEY_INPUT_D);
	AddInputCode(CommandKind::kRun,				KEY_INPUT_LSHIFT);
	AddInputCode(CommandKind::kSquat,			KEY_INPUT_LCONTROL);
	AddInputCode(CommandKind::kSquat,			KEY_INPUT_E);
	AddInputCode(CommandKind::kShot,			mouse::ButtonKind::kLeft);
	AddInputCode(CommandKind::kReadyGun,		mouse::ButtonKind::kRight);
	AddInputCode(CommandKind::kReloadGun,		KEY_INPUT_R);
	AddInputCode(CommandKind::kSilentKill,		mouse::ButtonKind::kLeft);
	AddInputCode(CommandKind::kQuickTurn,		KEY_INPUT_Q);
	AddInputCode(CommandKind::kMoveUpCamera,	mouse::SlideDirKind::kUp);
	AddInputCode(CommandKind::kMoveUpCamera,	KEY_INPUT_UP);
	AddInputCode(CommandKind::kMoveDownCamera,	mouse::SlideDirKind::kDown);
	AddInputCode(CommandKind::kMoveDownCamera,	KEY_INPUT_DOWN);
	AddInputCode(CommandKind::kMoveLeftCamera,	mouse::SlideDirKind::kLeft);
	AddInputCode(CommandKind::kMoveLeftCamera,	KEY_INPUT_LEFT);
	AddInputCode(CommandKind::kMoveRightCamera,	mouse::SlideDirKind::kRight);
	AddInputCode(CommandKind::kMoveRightCamera,	KEY_INPUT_RIGHT);
	AddInputCode(CommandKind::kInitAngle,		KEY_INPUT_Q);
}

void CommandManager::InitPadCommand()
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
	AddInputCode(CommandKind::kMoveUpPlayer,	pad::StickKind	::kLSUp);
	AddInputCode(CommandKind::kMoveDownPlayer,	pad::StickKind	::kLSDown);
	AddInputCode(CommandKind::kMoveLeftPlayer,	pad::StickKind	::kLSLeft);
	AddInputCode(CommandKind::kMoveRightPlayer,	pad::StickKind	::kLSRight);
	AddInputCode(CommandKind::kRun,				pad::ButtonKind	::kLSPush);
	AddInputCode(CommandKind::kSquat,			pad::ButtonKind	::kB);
	AddInputCode(CommandKind::kShot,			pad::TriggerKind::kRT);
	AddInputCode(CommandKind::kReadyGun,		pad::TriggerKind::kLT);
	AddInputCode(CommandKind::kReloadGun,		pad::ButtonKind	::kX);
	AddInputCode(CommandKind::kSilentKill,		pad::TriggerKind::kRT);
	AddInputCode(CommandKind::kQuickTurn,		pad::ButtonKind	::kRB);
	AddInputCode(CommandKind::kMoveUpCamera,	pad::StickKind	::kRSUp);
	AddInputCode(CommandKind::kMoveDownCamera,	pad::StickKind	::kRSDown);
	AddInputCode(CommandKind::kMoveLeftCamera,	pad::StickKind	::kRSLeft);
	AddInputCode(CommandKind::kMoveRightCamera,	pad::StickKind	::kRSRight);
	AddInputCode(CommandKind::kInitAngle,		pad::ButtonKind	::kRB);
}
