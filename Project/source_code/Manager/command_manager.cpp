#include "command_manager.hpp"

CommandManager::CommandManager()
{
	LoadSelectCommand();
	LoadPlayerCommand();
	LoadCameraCommand();

	InitKeyCommand();
	InitPadCommand();
}

CommandManager::~CommandManager()
{

}

void CommandManager::LoadSelectCommand()
{
	// TODO : Œã‚ÉJson‰»
	m_commands.emplace_back(std::make_shared<select_cmd::Decide>());
	m_commands.emplace_back(std::make_shared<select_cmd::Back>());
	m_commands.emplace_back(std::make_shared<select_cmd::Up>());
	m_commands.emplace_back(std::make_shared<select_cmd::Down>());
	m_commands.emplace_back(std::make_shared<select_cmd::Left>());
	m_commands.emplace_back(std::make_shared<select_cmd::Right>());
	m_commands.emplace_back(std::make_shared<select_cmd::Pause>());
}

void CommandManager::LoadPlayerCommand()
{
	// TODO : Œã‚ÉJson‰»
	m_commands.emplace_back(std::make_shared<player_cmd::MoveUp>());
	m_commands.emplace_back(std::make_shared<player_cmd::MoveDown>());
	m_commands.emplace_back(std::make_shared<player_cmd::MoveLeft>());
	m_commands.emplace_back(std::make_shared<player_cmd::MoveRight>());
	m_commands.emplace_back(std::make_shared<player_cmd::Run>());
	m_commands.emplace_back(std::make_shared<player_cmd::Squat>());
	m_commands.emplace_back(std::make_shared<player_cmd::Shot>());
	m_commands.emplace_back(std::make_shared<player_cmd::Ready>());
	m_commands.emplace_back(std::make_shared<player_cmd::Reload>());
	m_commands.emplace_back(std::make_shared<player_cmd::SilentKill>());
	m_commands.emplace_back(std::make_shared<player_cmd::QuickTurn>());
}

void CommandManager::LoadCameraCommand()
{
	// TODO : Œã‚ÉJson‰»
	m_commands.emplace_back(std::make_shared<camera_cmd::MoveUp>());
	m_commands.emplace_back(std::make_shared<camera_cmd::MoveDown>());
	m_commands.emplace_back(std::make_shared<camera_cmd::MoveLeft>());
	m_commands.emplace_back(std::make_shared<camera_cmd::MoveRight>());
	m_commands.emplace_back(std::make_shared<camera_cmd::InitAngle>());
}

void CommandManager::Execute(const CommandKind command_kind)
{

}

void CommandManager::InitKeyCommand()
{
	m_key_command.clear();

	// TODO : Œã‚ÉJson‰»
	AddCommand(CommandKind::kDecide,			KEY_INPUT_SPACE,			std::make_shared<select_cmd::Decide>());
	AddCommand(CommandKind::kDecide,			KEY_INPUT_F,				std::make_shared<select_cmd::Decide>());
	AddCommand(CommandKind::kBack,				mouse::ButtonKind::kRight,	std::make_shared<select_cmd::Back>());
	AddCommand(CommandKind::kSelectUp,			KEY_INPUT_W,				std::make_shared<select_cmd::Up>());
	AddCommand(CommandKind::kSelectUp,			KEY_INPUT_UP,				std::make_shared<select_cmd::Up>());
	AddCommand(CommandKind::kSelectUp,			mouse::WheelKind::kUp,		std::make_shared<select_cmd::Up>());
	AddCommand(CommandKind::kSelectDown,		KEY_INPUT_S,				std::make_shared<select_cmd::Down>());
	AddCommand(CommandKind::kSelectDown,		KEY_INPUT_DOWN,				std::make_shared<select_cmd::Down>());
	AddCommand(CommandKind::kSelectDown,		mouse::WheelKind::kDown,	std::make_shared<select_cmd::Down>());
	AddCommand(CommandKind::kSelectLeft,		KEY_INPUT_A,				std::make_shared<select_cmd::Left>());
	AddCommand(CommandKind::kSelectLeft,		KEY_INPUT_LEFT,				std::make_shared<select_cmd::Left>());
	AddCommand(CommandKind::kSelectRight,		KEY_INPUT_D,				std::make_shared<select_cmd::Right>());
	AddCommand(CommandKind::kSelectRight,		KEY_INPUT_RIGHT,			std::make_shared<select_cmd::Right>());
	AddCommand(CommandKind::kPause,				KEY_INPUT_ESCAPE,			std::make_shared<select_cmd::Pause>());
	AddCommand(CommandKind::kPause,				KEY_INPUT_TAB,				std::make_shared<select_cmd::Pause>());
	AddCommand(CommandKind::kMoveUpPlayer,		KEY_INPUT_W,				std::make_shared<player_cmd::MoveUp>());
	AddCommand(CommandKind::kMoveDownPlayer,	KEY_INPUT_S,				std::make_shared<player_cmd::MoveDown>());
	AddCommand(CommandKind::kMoveLeftPlayer,	KEY_INPUT_A,				std::make_shared<player_cmd::MoveLeft>());
	AddCommand(CommandKind::kMoveRightPlayer,	KEY_INPUT_D,				std::make_shared<player_cmd::MoveRight>());
	AddCommand(CommandKind::kRun,				KEY_INPUT_LSHIFT,			std::make_shared<player_cmd::Run>());
	AddCommand(CommandKind::kSquat,				KEY_INPUT_LCONTROL,			std::make_shared<player_cmd::Squat>());
	AddCommand(CommandKind::kSquat,				KEY_INPUT_E,				std::make_shared<player_cmd::Squat>());
	AddCommand(CommandKind::kShot,				mouse::ButtonKind::kLeft,	std::make_shared<player_cmd::Shot>());
	AddCommand(CommandKind::kReadyGun,			mouse::ButtonKind::kRight,	std::make_shared<player_cmd::Ready>());
	AddCommand(CommandKind::kReloadGun,			KEY_INPUT_R,				std::make_shared<player_cmd::Reload>());
	AddCommand(CommandKind::kSilentKill,		mouse::ButtonKind::kLeft,	std::make_shared<player_cmd::SilentKill>());
	AddCommand(CommandKind::kQuickTurn,			KEY_INPUT_Q,				std::make_shared<player_cmd::QuickTurn>());
	AddCommand(CommandKind::kMoveUpCamera,		mouse::SlideDirKind::kUp,	std::make_shared<camera_cmd::MoveUp>());
	AddCommand(CommandKind::kMoveUpCamera,		KEY_INPUT_UP,				std::make_shared<camera_cmd::MoveUp>());
	AddCommand(CommandKind::kMoveDownCamera,	mouse::SlideDirKind::kDown,	std::make_shared<camera_cmd::MoveDown>());
	AddCommand(CommandKind::kMoveDownCamera,	KEY_INPUT_DOWN,				std::make_shared<camera_cmd::MoveDown>());
	AddCommand(CommandKind::kMoveLeftCamera,	mouse::SlideDirKind::kLeft, std::make_shared<camera_cmd::MoveLeft>());
	AddCommand(CommandKind::kMoveLeftCamera,	KEY_INPUT_LEFT,				std::make_shared<camera_cmd::MoveLeft>());
	AddCommand(CommandKind::kMoveRightCamera,	mouse::SlideDirKind::kRight,std::make_shared<camera_cmd::MoveRight>());
	AddCommand(CommandKind::kMoveRightCamera,	KEY_INPUT_RIGHT,			std::make_shared<camera_cmd::MoveRight>());
	AddCommand(CommandKind::kInitAngle,			KEY_INPUT_Q,				std::make_shared<camera_cmd::InitAngle>());
}

void CommandManager::InitPadCommand()
{
	m_pad_command.clear();

	// TODO : Œã‚ÉJson‰»
	AddCommand(CommandKind::kDecide,			pad::ButtonKind	::kA,		std::make_shared<select_cmd::Decide>());
	AddCommand(CommandKind::kBack,				pad::ButtonKind	::kB,		std::make_shared<select_cmd::Back>());
	AddCommand(CommandKind::kSelectUp,			pad::ButtonKind	::kUp,		std::make_shared<select_cmd::Up>());
	AddCommand(CommandKind::kSelectUp,			pad::StickKind	::kLSUp,	std::make_shared<select_cmd::Up>());
	AddCommand(CommandKind::kSelectDown,		pad::ButtonKind	::kDown,	std::make_shared<select_cmd::Down>());
	AddCommand(CommandKind::kSelectDown,		pad::StickKind	::kLSDown,	std::make_shared<select_cmd::Down>());
	AddCommand(CommandKind::kSelectLeft,		pad::ButtonKind	::kLeft,	std::make_shared<select_cmd::Left>());
	AddCommand(CommandKind::kSelectLeft,		pad::StickKind	::kLSLeft,	std::make_shared<select_cmd::Left>());
	AddCommand(CommandKind::kSelectRight,		pad::ButtonKind	::kRight,	std::make_shared<select_cmd::Right>());
	AddCommand(CommandKind::kSelectRight,		pad::StickKind	::kLSRight,	std::make_shared<select_cmd::Right>());
	AddCommand(CommandKind::kPause,				pad::ButtonKind	::kStart,	std::make_shared<select_cmd::Pause>());
	AddCommand(CommandKind::kMoveUpPlayer,		pad::StickKind	::kLSUp,	std::make_shared<player_cmd::MoveUp>());
	AddCommand(CommandKind::kMoveDownPlayer,	pad::StickKind	::kLSDown,	std::make_shared<player_cmd::MoveDown>());
	AddCommand(CommandKind::kMoveLeftPlayer,	pad::StickKind	::kLSLeft,	std::make_shared<player_cmd::MoveLeft>());
	AddCommand(CommandKind::kMoveRightPlayer,	pad::StickKind	::kLSRight,	std::make_shared<player_cmd::MoveRight>());
	AddCommand(CommandKind::kRun,				pad::ButtonKind	::kLSPush,	std::make_shared<player_cmd::Run>());
	AddCommand(CommandKind::kSquat,				pad::ButtonKind	::kB,		std::make_shared<player_cmd::Squat>());
	AddCommand(CommandKind::kShot,				pad::TriggerKind::kRT,		std::make_shared<player_cmd::Shot>());
	AddCommand(CommandKind::kReadyGun,			pad::TriggerKind::kLT,		std::make_shared<player_cmd::Ready>());
	AddCommand(CommandKind::kReloadGun,			pad::ButtonKind	::kX,		std::make_shared<player_cmd::Reload>());
	AddCommand(CommandKind::kSilentKill,		pad::TriggerKind::kRT,		std::make_shared<player_cmd::SilentKill>());
	AddCommand(CommandKind::kQuickTurn,			pad::ButtonKind	::kRB,		std::make_shared<player_cmd::QuickTurn>());
	AddCommand(CommandKind::kMoveUpCamera,		pad::StickKind	::kRSUp,	std::make_shared<camera_cmd::MoveUp>());
	AddCommand(CommandKind::kMoveDownCamera,	pad::StickKind	::kRSDown,	std::make_shared<camera_cmd::MoveDown>());
	AddCommand(CommandKind::kMoveLeftCamera,	pad::StickKind	::kRSLeft,	std::make_shared<camera_cmd::MoveLeft>());
	AddCommand(CommandKind::kMoveRightCamera,	pad::StickKind	::kRSRight, std::make_shared<camera_cmd::MoveRight>());
	AddCommand(CommandKind::kInitAngle,			pad::ButtonKind	::kRB,		std::make_shared<camera_cmd::InitAngle>());
}
