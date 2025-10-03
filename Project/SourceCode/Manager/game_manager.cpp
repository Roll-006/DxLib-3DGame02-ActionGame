#include "game_manager.hpp"

GameManager::GameManager():
	m_game_system_setter(std::make_unique<GameSystemSetter>()),
	m_window			(std::make_unique<Window>()),
	m_scene_manager		(nullptr)
{
	HandleCreator			::Generate();
	HandleKeeper			::Generate();
	CommandHandler			::Generate();
	Debugger				::Generate();
	EventSystem				::Generate();
	ObjectPoolHolder		::Generate();
	UIDrawer				::Generate();

	GameTimeManager			::Generate();
	ObjManager				::Generate();
	CollisionManager		::Generate();
	PhysicsManager			::Generate();
	CinemachineBrain		::Generate();
	EffectManager			::Generate();
	RifleCartridgeManager	::Generate();

	m_scene_manager = std::make_unique<SceneManager>();
}

GameManager::~GameManager()
{	
	HandleCreator			::Delete();
	HandleKeeper			::Delete();
	CommandHandler			::Delete();
	Debugger				::Delete();
	EventSystem				::Delete();
	ObjectPoolHolder		::Delete();
	UIDrawer				::Delete();

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

		GameTimeManager::GetInstance()->Draw();
		GameTimeManager::GetInstance()->WaitTime();
		ScreenFlip();
	}
}

bool GameManager::IsContinueLoop()
{
	if (ProcessMessage()  != 0) { return false; }
	if (ClearDrawScreen() != 0) { return false; }

	//#ifdef _DEBUG
	// TODO : Œã‚ÉƒRƒ}ƒ“ƒh‰»
	//if (InputChecker::GetInstance()->IsInput(KEY_INPUT_ESCAPE)) { return false; }

	//if (   InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kLB)
	//	&& InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kRB)
	//	&& InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kLSPush)
	//	&& InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kRSPush)
	//	&& InputChecker::GetInstance()->IsInput(pad::TriggerKind::kRT)
	//	&& InputChecker::GetInstance()->IsInput(pad::TriggerKind::kRT))
	//{
	//	return false;
	//}
	//#endif

	return true;
}
