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
	ObjAccessor				::Generate();
	CollisionManager		::Generate();
	PhysicsManager			::Generate();
	CinemachineBrain		::Generate();
	EffectManager			::Generate();
	RifleCartridgeManager	::Generate();

	m_scene_manager = std::make_unique<SceneManager>();

	// イベント登録
	EventSystem::GetInstance()->Subscribe<ExitGameEvent>(this, &GameManager::ExitGame);
}

GameManager::~GameManager()
{	
	m_scene_manager = nullptr;

	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<ExitGameEvent>(this, &GameManager::ExitGame);

	HandleCreator			::Delete();
	CommandHandler			::Delete();
	InputChecker			::Delete();
	Debugger				::Delete();
	UIDrawer				::Delete();
	FontHandler				::Delete();
	SceneFader				::Delete();

	GameTimeManager			::Delete();
	CollisionManager		::Delete();
	PhysicsManager			::Delete();
	RifleCartridgeManager	::Delete();

	CinemachineBrain		::Delete();
	TabDrawer				::Delete();
	LightHolder				::Delete();
	EffectManager			::Delete();
	ObjectPoolHolder		::Delete();
	ObjAccessor				::Delete();
	EventSystem				::Delete();
	HandleKeeper			::Delete();
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

		GameTimeManager::GetInstance()->WaitTime();
		ScreenFlip();
	}
}

void GameManager::ExitGame(const ExitGameEvent& event)
{
	m_is_exit_game = true;
}

bool GameManager::IsContinueLoop() const
{
	if (ProcessMessage()  != 0) { return false; }
	if (ClearDrawScreen() != 0) { return false; }
	if (m_is_exit_game)			{ return false; }

	return true;
}
