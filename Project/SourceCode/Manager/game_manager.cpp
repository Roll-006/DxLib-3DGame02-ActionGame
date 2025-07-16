#include "game_manager.hpp"

GameManager::GameManager():
	m_game_system_setter(std::make_unique<GameSystemSetter>()),
	m_window			(std::make_unique<Window>()),
	m_fps				(std::make_unique<FPS>()),
	m_scene_manager		(nullptr)
{
	HandleKeeper			::Generate();	
	UIManager				::Generate();
	CommandHandler			::Generate();
	ObjManager				::Generate();
	CollisionManager		::Generate();
	PhysicsManager			::Generate();
	CameraManager			::Generate();
	ObjectPoolManager		::Generate();
	RifleCartridgeManager	::Generate();

	m_scene_manager = std::make_unique<SceneManager>();
}

GameManager::~GameManager()
{	
	HandleKeeper			::Delete();	
	UIManager				::Delete();
	CommandHandler			::Delete();
	ObjManager				::Delete();
	CollisionManager		::Delete();
	PhysicsManager			::Delete();
	CameraManager			::Delete();
	ObjectPoolManager		::Delete();
	RifleCartridgeManager	::Delete();
}

void GameManager::Run()
{
	while (IsContinueLoop())
	{
		m_fps	->Update();
		m_window->Update();

		m_scene_manager->Update();
		m_scene_manager->LateUpdate();
		m_scene_manager->Draw();

		m_fps->Draw();

		m_fps->Wait();
		ScreenFlip();
	}
}

bool GameManager::IsContinueLoop()
{
	if (ProcessMessage()  != 0) { return false; }
	if (ClearDrawScreen() != 0) { return false; }

	#ifdef _DEBUG
	// TODO : Œã‚ÉƒRƒ}ƒ“ƒh‰»
	if (InputChecker::GetInstance()->IsInput(KEY_INPUT_ESCAPE)) { return false; }

	if (   InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kLB)
		&& InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kRB)
		&& InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kLSPush)
		&& InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kRSPush)
		&& InputChecker::GetInstance()->IsInput(pad::TriggerKind::kRT)
		&& InputChecker::GetInstance()->IsInput(pad::TriggerKind::kRT))
	{
		return false;
	}
	#endif

	return true;
}
