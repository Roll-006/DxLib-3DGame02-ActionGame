#include "game_manager.hpp"

GameManager::GameManager():
	m_game_system_setter(std::make_unique<GameSystemSetter>()),
	m_window			(std::make_unique<Window>()),
	m_scene_manager		(nullptr),
	m_is_exit_game		(false)
{
	HandleCreator			::Generate();
	HandleKeeper			::Generate();
	CommandHandler			::Generate();
	InputChecker			::Generate();
	Debugger				::Generate();
	EventSystem				::Generate();
	ObjectPoolHolder		::Generate();
	UIDrawer				::Generate();
	TabDrawer				::Generate();
	FontHandler				::Generate();
	LightHolder				::Generate();
	SceneFader				::Generate();

	GameTimeManager			::Generate();
	ObjManager				::Generate();
	CollisionManager		::Generate();
	PhysicsManager			::Generate();
	CinemachineBrain		::Generate();
	EffectManager			::Generate();
	RifleCartridgeManager	::Generate();

	m_scene_manager = std::make_unique<SceneManager>();

	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Subscribe<ExitGameEvent>(this, &GameManager::ExitGame);
}

GameManager::~GameManager()
{	
	// ƒCƒxƒ“ƒg‚Ì“o˜^‰ğœ
	EventSystem::GetInstance()->Unsubscribe<ExitGameEvent>(this, &GameManager::ExitGame);

	HandleCreator			::Delete();
	HandleKeeper			::Delete();
	CommandHandler			::Delete();
	InputChecker			::Delete();
	Debugger				::Delete();
	EventSystem				::Delete();
	ObjectPoolHolder		::Delete();
	UIDrawer				::Delete();
	TabDrawer				::Delete();
	FontHandler				::Delete();
	LightHolder				::Delete();
	SceneFader				::Delete();

	GameTimeManager			::Delete();
	ObjManager				::Delete();
	CollisionManager		::Delete();
	PhysicsManager			::Delete();
	CinemachineBrain		::Delete();
	EffectManager			::Delete();
	RifleCartridgeManager	::Delete();
}

void GameManager::Run()
{
	while (IsContinueLoop())
	{
		GameTimeManager::GetInstance()->Update();
		m_window->Update();

		m_scene_manager->Update();
		m_scene_manager->LateUpdate();
		m_scene_manager->DrawToShadowMap();
		m_scene_manager->Draw();

		//GameTimeManager::GetInstance()->Draw();
		GameTimeManager::GetInstance()->WaitTime();
		ScreenFlip();
	}
}

void GameManager::ExitGame(const ExitGameEvent& event)
{
	m_is_exit_game = true;
}

bool GameManager::IsContinueLoop()
{
	if (ProcessMessage()  != 0) { return false; }
	if (ClearDrawScreen() != 0) { return false; }
	if (m_is_exit_game)			{ return false; }

	return true;
}
