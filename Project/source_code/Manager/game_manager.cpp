#include "game_manager.hpp"

bool GameManager::m_instantiated = false;

GameManager::GameManager():
	m_window			(nullptr),
	m_fps				(nullptr),
	m_scene_obj_manager	(nullptr)
{
	assert(!m_instantiated);
	m_instantiated  = true;

	SetUpGameSystem();

	InputChecker  ::Generate();
	GameObjManager::Generate();

	m_window			= std::make_unique<Window>();
	m_fps				= std::make_unique<FPS>();
	m_scene_obj_manager = std::make_unique<SceneObjManager>();
}

GameManager::~GameManager()
{
	m_instantiated = false;

	InputChecker  ::Delete();
	GameObjManager::Delete();
	
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
		m_scene_obj_manager->Update();
		m_scene_obj_manager->Draw();
		m_fps->Wait();

		ScreenFlip();
	}
}

void GameManager::SetUpGameSystem()
{
	SetGraphMode(Window::kWidth, Window::kHeight, 32);
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
