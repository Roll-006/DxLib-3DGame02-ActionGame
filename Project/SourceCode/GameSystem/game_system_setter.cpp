#include "game_system_setter.hpp"

GameSystemSetter::GameSystemSetter()
{
	SetUpGameSystem();

	RandomGenerator	::Generate();
	InputChecker	::Generate();
}

GameSystemSetter::~GameSystemSetter()
{
	RandomGenerator	::Delete();
	InputChecker	::Delete();

	Effkseer_End();
	DxLib_End();
}

void GameSystemSetter::SetUpGameSystem()
{
	// 基本設定
	SetGraphMode(Window::kWidth, Window::kHeight, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) { exit(EXIT_FAILURE); }
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if (Effekseer_Init(8000) == -1) { exit(EXIT_FAILURE); } // TODO : 定数化
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	SetMainWindowText("3DGAME");

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
	SetFogEnable(TRUE);
	SetFogMode(DX_FOGMODE_LINEAR);
	SetFogColor(195, 207, 219);
	SetFogStartEnd(300.f, 3100.0f);
	SetFogDensity(0.1f);
}
