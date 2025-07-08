#include "game_manager.hpp"

GameManager::GameManager():
	m_window		(std::make_unique<Window>()),
	m_fps			(std::make_unique<FPS>()),
	m_scene_manager	(nullptr)
{
	SetUpGameSystem();

	RandomGenerator			::Generate();
	HandleKeeper			::Generate();
	InputChecker			::Generate();
	UIManager				::Generate();
	CommandHandler			::Generate();
	ObjManager				::Generate();
	CollisionManager		::Generate();
	PhysicsManager			::Generate();
	ObjectPoolManager		::Generate();
	RifleCartridgeManager	::Generate();

	m_scene_manager = std::make_unique<SceneObjManager>();
}

GameManager::~GameManager()
{
	RandomGenerator			::Delete();
	HandleKeeper			::Delete();
	InputChecker			::Delete();
	UIManager				::Delete();
	CommandHandler			::Delete();
	ObjManager				::Delete();
	CollisionManager		::Delete();
	PhysicsManager			::Delete();
	ObjectPoolManager		::Delete();
	RifleCartridgeManager	::Delete();
	
	Effkseer_End();
	DxLib_End();
}

void GameManager::Run()
{
	while (IsContinueLoop())
	{
		m_fps->Update();
		m_window->Update();

		m_scene_manager->Update();
		m_scene_manager->LateUpdate();
		m_scene_manager->Draw();

		m_fps->Draw();

		// TEST : 仮でグリッドを描画
		//DxLibHelper::DrawYPlaneGrid(FLOAT2(50, 50), 100);
		//DrawLine3D(v3d::GetZeroV(), axis::GetWorldXAxis() * 10000, 0xff0000);
		//DrawLine3D(v3d::GetZeroV(), axis::GetWorldYAxis() * 10000, 0x00ff22);
		//DrawLine3D(v3d::GetZeroV(), axis::GetWorldZAxis() * 10000, 0x0077ff);

		m_fps->Wait();

		ScreenFlip();
	}
}

void GameManager::SetUpGameSystem()
{
	// 基本設定
	SetGraphMode(Window::kWidth, Window::kHeight, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1){ exit(EXIT_FAILURE); }
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if(Effekseer_Init(8000) == -1){ exit(EXIT_FAILURE); }
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Zバッファ
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	// ライト
	SetUseLighting(TRUE);
	//SetGlobalAmbientLight();
	//SetLightDirection();
	//SetLightDifColor();
	//SetLightAmbColor();

	// フォグ
	SetFogEnable	(TRUE);
	SetFogMode		(DX_FOGMODE_LINEAR);
	SetFogColor		(195, 207, 219);
	SetFogStartEnd	(300.f, 3100.0f);
	SetFogDensity	(0.1f);

	SetMainWindowText("3DGame Sample");
}

bool GameManager::IsContinueLoop()
{
	if (ProcessMessage()  != 0) { return false; }
	if (ClearDrawScreen() != 0) { return false; }

	#ifdef _DEBUG
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
