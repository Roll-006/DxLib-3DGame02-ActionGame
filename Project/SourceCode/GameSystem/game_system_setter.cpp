#include "game_system_setter.hpp"

GameSystemSetter::GameSystemSetter()
{
	SetUpGameSystem();

	RandomGenerator	::Generate();
}

GameSystemSetter::~GameSystemSetter()
{
	RandomGenerator	::Delete();

	Effkseer_End();
	DxLib_End();
}

void GameSystemSetter::SetUpGameSystem()
{
	// 基本設定
	SetOutApplicationLogValidFlag(FALSE);
	SetGraphMode(Window::kScreenSize.x, Window::kScreenSize.y, 32);
	ChangeWindowMode(TRUE);
	Set3DSoundOneMetre(30.0f);
	if (DxLib_Init() == -1) { exit(EXIT_FAILURE); }
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if (Effekseer_Init(8000) == -1) { exit(EXIT_FAILURE); }
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	SetMainWindowText("3DGAME");

	// Zバッファ
	SetUseZBuffer3D  (TRUE);
	SetWriteZBuffer3D(TRUE);

	// フォグ
	SetFogEnable(TRUE);
	SetFogMode(DX_FOGMODE_LINEAR);
	SetFogColor(48, 49, 56);
	SetFogStartEnd(200.f, 1700.0f);
	SetFogDensity(0.1f);

	// フォント
	SetFontUseAdjustSizeFlag(FALSE);
}
