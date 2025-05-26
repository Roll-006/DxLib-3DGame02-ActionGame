#include "game_manager.hpp"

GameManager::GameManager():
	m_window		(std::make_unique<Window>()),
	m_fps			(std::make_unique<FPS>()),
	m_scene_manager	(std::make_unique<SceneObjManager>())
{
	SetUpGameSystem();

	InputChecker	::Generate();
	ObjManager		::Generate();
	CollisionManager::Generate();
	PhysicsManager	::Generate();
}

GameManager::~GameManager()
{
	InputChecker	::Delete();
	ObjManager		::Delete();
	CollisionManager::Delete();
	PhysicsManager	::Delete();
	
	Effkseer_End();
	DxLib_End();
}

void GameManager::Run()
{
	while (ContinueLoop())
	{
		m_fps->Update();
		m_window->Update();
		m_scene_manager->Update();
		m_scene_manager->Draw();
		m_fps->Wait();

		ScreenFlip();
	}
}

void GameManager::SetUpGameSystem()
{
	SetGraphMode(Window::kWidth, Window::kHeight, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1){ exit(EXIT_FAILURE); }
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if(Effekseer_Init(8000) == -1){ exit(EXIT_FAILURE); }
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetMainWindowText("3DGame Sample");
}

bool GameManager::ContinueLoop()
{
	if (ProcessMessage()  != 0) { return false; }
	if (ClearDrawScreen() != 0) { return false; }

	#ifdef _DEBUG
	if (InputChecker::GetInstance()->IsInput(KEY_INPUT_ESCAPE)) { return false; }

	if (   InputChecker::GetInstance()->IsInput(pad::ButtonKind::kLB)
		&& InputChecker::GetInstance()->IsInput(pad::ButtonKind::kRB)
		&& InputChecker::GetInstance()->IsInput(pad::ButtonKind::kLSPush)
		&& InputChecker::GetInstance()->IsInput(pad::ButtonKind::kRSPush)
		&& InputChecker::GetInstance()->IsInput(pad::TriggerKind::kRT)
		&& InputChecker::GetInstance()->IsInput(pad::TriggerKind::kRT))
	{
		return false;
	}
	#endif

	return true;
}
