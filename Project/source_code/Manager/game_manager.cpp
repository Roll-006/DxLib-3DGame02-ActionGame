#include "game_manager.hpp"

GameManager::GameManager():
	m_window		(nullptr),
	m_fps			(nullptr),
	m_scene_manager	(nullptr)
{
	SetUpGameSystem();

	InputChecker::Generate();
	ObjManager	::Generate();

	m_window		= std::make_unique<Window>();
	m_fps			= std::make_unique<FPS>();
	m_scene_manager = std::make_unique<SceneObjManager>();
}

GameManager::~GameManager()
{
	InputChecker::Delete();
	ObjManager	::Delete();
	
	Effkseer_End();
	DxLib_End();
}

void GameManager::Run()
{
	while (ContinueLoop())
	{
		m_fps->Update();
		m_window->Update();
		InputChecker::GetInstance()->Update();
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
	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) { return false; }
	#endif

	return true;
}
